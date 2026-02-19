This project will generate
* An enum
* A mapping from string -> enum
* A mapping from enum -> string

This is helpful if you frequently need to make those, or if you frequently change the enum. <br>
You can do this with macros, but those flood the namespace and precious brainpower to write.

Run with the name of the input file as the first argument.

### General Format: <br>
```
<name_of_enum> <underlying_type>

	<enumerator> <string_representation> OR !BEGIN <category_name> OR !END <category_name>

!FINISH
```

Categories are optional, define them if you want to do some fast enum checking like so: <br>
```c++
    if(e > BEGIN_CARS && e < END_CARS) {
    ...
    }
    //excluding the required casts for readability
```

If you define a category, make sure to include the ending. If you don't, nothing bad will happen, the formatting will just look ugly. <br>
End the definition with !FINISH. <br>

Manually changing the enum / mapping after the fact is risky, as the mapping from enum to string heavily 
relies on the order of declaration. I'd recommend you keep the input file around as the formal definition of your enum,
change it there, and then re-run this program

### Example:


The input file:

```
Vehicles unsigned

!BEGIN CARS 

RAV4 rav_four
CAMRY toyota_camry
MODEL_Y model_y

!END CARS

!BEGIN TRUCKS 

F_150 f150
SIERRA sierra
SILVERADO silverado

!END TRUCKS

!FINISH
```

Generates the header Vehicles_generated.hpp:

```c++
#pragma once
#include <string>
#include <unordered_map>
#include <utility>
enum class Vehicles : unsigned {

  BEGIN_CARS,
  RAV4,
  CAMRY,
  MODEL_Y,
  END_CARS,

  BEGIN_TRUCKS,
  F_150,
  SIERRA,
  SILVERADO,
  END_TRUCKS,

};

inline const std::unordered_map<std::string, Vehicles> stringToVehicles{

    {"!BEGIN_CARS", Vehicles::BEGIN_CARS},
    {"rav_four", Vehicles::RAV4},
    {"toyota_camry", Vehicles::CAMRY},
    {"model_y", Vehicles::MODEL_Y},
    {"!END_CARS", Vehicles::END_CARS},

    {"!BEGIN_TRUCKS", Vehicles::BEGIN_TRUCKS},
    {"f150", Vehicles::F_150},
    {"sierra", Vehicles::SIERRA},
    {"silverado", Vehicles::SILVERADO},
    {"!END_TRUCKS", Vehicles::END_TRUCKS},

};

constexpr const char *vehiclesToString(Vehicles e) {
  constexpr const char *toString[] = {

      "!BEGIN_CARS",   "rav_four", "toyota_camry", "model_y",   "!END_CARS",

      "!BEGIN_TRUCKS", "f150",     "sierra",       "silverado", "!END_TRUCKS",

  };

  return toString[std::to_underlying(e)];
}
```




