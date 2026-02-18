#pragma once
#include "Types.hh"
#include "ComponentPool.hh"
#include <array>
#include <bitset>
#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <cassert>
#include <algorithm>

namespace ECS {
  struct EntityRecord {
    std::bitset<MAX_COMPONENTS> componentsMask;
  };

  class Registry {
  private:
    std::vector<EntityRecord> mEntityRecords; // Indexed by EntityID
    std::vector<EntityID> mFreeList;  // Recycled entity IDs
    EntityID mNextEntity = 0;

    // Component pools Indexed by ComponentID
    std::array<std::unique_ptr<IComponentPool>, MAX_COMPONENTS> mComponentPools;
    mutable std::unordered_map<std::type_index, ComponentID> mComponentIDs;
    mutable ComponentID mNextComponentID = 0;

  public:
    EntityID CreateEntity()
    {
      EntityID entity;

      // Check free list first
      if (!mFreeList.empty()) {
        entity = mFreeList.back();
        mFreeList.pop_back();

        // Reset component mask for reused entity
        mEntityRecords[entity].componentsMask.reset();
      } else {
        entity = mNextEntity++;
        mEntityRecords.resize(entity + 1);
      }

      return entity;
    }

    void DestroyEntity(EntityID entity)
    {
      if (!IsValid(entity)) return;

      // Remove all components for this entity
      for (ComponentID id = 0; id < MAX_COMPONENTS; ++id) {
        if (mEntityRecords[entity].componentsMask.test(id)) {
          // Remove from pool or clear mask and let pool handle cleanup later
          mComponentPools[id]->Remove(entity);
          mEntityRecords[entity].componentsMask.reset(id);
        }
      }

      // Add to free list
      mFreeList.push_back(entity);

      // Clear component mask
      mEntityRecords[entity].componentsMask.reset();
    }

    bool IsValid(EntityID entity) const {
      // Check bounds
      if (entity >= mEntityRecords.size()) return false;

      // Check if in free list
      if (entity >= mNextEntity) return false;

      // Check if in free list
      return std::find(mFreeList.begin(), mFreeList.end(), entity) == mFreeList.end();
    }

    template<typename T>
    T& AddComponent(EntityID entity, const T& component)
    {
      auto& pool = GetOrCreatePool<T>();
      mEntityRecords[entity].componentsMask.set(GetComponentID<T>());
      return pool.Add(entity, component);
    }

    template<typename T>
    void RemoveComponent(EntityID entity)
    {
      GetPool<T>().Remove(entity);
      mEntityRecords[entity].componentsMask.reset(GetComponentID<T>());
    }

    template<typename T>
    T* GetComponent(EntityID entity)
    {
      return GetPool<T>().Get(entity);
    }

    template<typename T>
    bool HasComponent(EntityID entity) const
    {
      return mEntityRecords[entity].componentsMask.test(GetComponentID<T>());
    }

    // Query entities with specific components
    template<typename... Components>
    std::vector<EntityID> Query()
    {
      // TODO: Return all entities that have ALL specified components
      std::vector<EntityID> results;

      for (EntityID entity = 0; entity < mEntityRecords.size(); ++entity) {
        if (!IsValid(entity)) continue;

        bool hasAll = (HasComponent<Components>(entity) && ...);

        if (hasAll) {
          results.push_back(entity);
        }
      }
      return results;
    }

  private:
    template<typename T>
    ComponentPool<T>& GetOrCreatePool()
    {
      // TODO: Lazy init of component pools
      // 1. Get component type ID
      ComponentID id = GetComponentID<T>();
      // 2. If pool doesn't exist, create it
      if (!mComponentPools[id]) {
        mComponentPools[id] = std::make_unique<ComponentPool<T>>();
      }
      // 3. Return reference to pool
      return *static_cast<ComponentPool<T>*>(mComponentPools[id].get());
    }

    template<typename T>
    ComponentPool<T>& GetPool()
    {
      ComponentID id = GetComponentID<T>();
      return *static_cast<ComponentPool<T>*>(mComponentPools[id].get());
    }

    template<typename T>
    ComponentID GetComponentID() const
    {
      // Create a type_index for component type
      std::type_index typeIndex(typeid(T));

      // Check if we already have an ID for this type
      auto it = mComponentIDs.find(typeIndex);
      if (it != mComponentIDs.end()) {
        return it->second;
      }

      // If not, assign new ID from mNextComponentID
      assert(mNextComponentID < MAX_COMPONENTS && "Too many component types!");

      // Assign new IDs
      ComponentID newID = mNextComponentID++;

      // Store it for future lookups
      mComponentIDs[typeIndex] = newID;

      // 3. Return the ID
      return newID;
    }
  };
}
