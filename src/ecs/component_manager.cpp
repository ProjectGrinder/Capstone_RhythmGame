#include <algorithm>
#include "ecs.h"  

void ECS::ComponentManager::entity_destroyed(ECS::Entity entity)  
{  
   std::for_each
   (
       component_stores.begin(), 
       component_stores.end(),  
       [&entity](auto& pair)  
       {  
           pair.second->entity_destroyed(entity);
       }
   );  
}