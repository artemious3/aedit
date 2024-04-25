

#include "Constructor.h"
#include "BaseEffect.h"
#include "Chorus.h"
#include "Gain.h"
#include "PassEQ.h"
#include "Pitch.h"
#include "Reverb.h"
#include "Robotiser.h"
#include "TimePitch.h"
#include "Reverb.h"

BaseEffect* Constructor::getEffect(const std::string& name){
    BaseEffect * effect = nullptr;
    
    if(name == "Gain"){
        effect = new Gain{};
    } else if(name == "Low/High-pass"){
        effect = new PassEQ{};
    } else if(name == "Robotiser"){
        effect = new Robotiser{};
    } else if(name == "Chorus"){
        effect = new Chorus{};
    } else if(name == "Pitch"){
        effect = new Pitch{};
    } else if (name == "TimePitch"){
        effect = new TimePitch{};
    } else if (name == "Reverb"){
        effect = new Reverb{};
    }


    return effect;
}