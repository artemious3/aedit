

#include "Constructor.h"
#include "BaseEffect.h"
#include "Gain.h"
#include "EQ.h"

BaseEffect* Constructor::getEffect(const std::string& name){
    BaseEffect * effect = nullptr;
    
    if(name == "Gain"){
        effect = new Gain{};
    } else if(name == "EQ"){
        effect = new EQ{};
    }

    return effect;
}