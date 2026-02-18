#pragma once
#include "Types.hh"
#include <vector>
#include <unordered_map>
#include <cassert>

namespace ECS {
  // Base class for type erasure
  class IComponentPool {
  public:
    virtual ~IComponentPool() = default;
    virtual void Remove(EntityID entity) = 0;
  };

  template<typename T>
  class ComponentPool : public IComponentPool {
  private:
    std::vector<T> mComponents; // Dense array of components
    std::vector<EntityID> mEntities;  // Parallel array of entity IDs
    std::unordered_map<EntityID, uint32_t> mEntityToIndex;  // Sparse index mapping

  public:
    T& Add(EntityID entity, const T& component) 
    {
      // Step 1: Safety check - make sure entity doesn't already have this component
      assert(!Has(entity) && "Entity already has this component!");
      
      // Step 2: Get the index where we'll store this component
      // This is just the current size of our dense array
      uint32_t index = mComponents.size();
      
      // Step 3: Add the component to the dense array
      // push_back adds to the end and grows the vector if needed
      mComponents.push_back(component);
      
      // Step 4: Add the entity ID to the parallel array at the same index
      // This keeps track of which entity owns which component
      mEntities.push_back(entity);
      
      // Step 5: Map the entity to its index in the dense array
      // This is our "sparse" mapping - allows O(1) lookup by entity ID
      mEntityToIndex[entity] = index;
      
      // Step 6: Return a reference to the newly added component
      // This lets the caller modify the component if needed
      return mComponents.back();
    }

    void Remove(EntityID entity) override
    {
      if (!Has(entity)) return;

      uint32_t indexToRemove = mEntityToIndex[entity];
      uint32_t lastIndex = mComponents.size() - 1;

      if (indexToRemove != lastIndex) {
        mComponents[indexToRemove] = std::move(mComponents[lastIndex]);
        mEntities[indexToRemove] = mEntities[lastIndex];
        mEntityToIndex[mEntities[lastIndex]] = indexToRemove;
      }

      mComponents.pop_back();
      mEntities.pop_back();
      mEntityToIndex.erase(entity);
    }
    
    T* Get(EntityID entity) 
    {
      // Step 1: Check if entity has this component
      if (!Has(entity)) {
        return nullptr;
      }
      
      // Step 2: Look up the index in our sparse map
      uint32_t index = mEntityToIndex[entity];
      
      // Step 3: Return pointer to component at that index
      return &mComponents[index];
    }

    bool Has(EntityID entity) const
    {
      // Check if entity exists in our sparse mapping
      // If it's in the map, the entity has this component
      return mEntityToIndex.find(entity) != mEntityToIndex.end();
    }

    // Iterator for cache-friendly iteration
    auto begin() { return mComponents.begin(); }
    auto end() { return mComponents.end(); }

    size_t Size() const { return mComponents.size(); }
  };
}
