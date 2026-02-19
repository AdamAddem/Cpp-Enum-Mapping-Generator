This project will generate
* An enum
* A mapping from string -> enum
* A mapping from enum -> string

This is helpful if you frequently need to make those, or if you frequently change the enum. <br>
You can do this with macros, but those flood the namespace and require precious brainpower to write. <br>
I wrote this for personal use, but if you want a feature just let me know and i'll be sure to get to it sometime this decade. <br>

Run with the name of the input file as the first argument, and an optional second argument w/ the name and extension of the output file.

### General Format: <br>
```
<name_of_enum> <namespace>

	<enumerator> (<string_representation> | -)
                OR 
      !BEGIN <category_name> 
                OR 
      !END <category_name>
  ...

!FINISH
```


\<name_of_enum\> does what you think it does, and \<namespace\> specifies the namespace to wrap the header in. <br>

String representations starting with '-' will have that '-' removed <br>
  * To specify an empty string, just use a single - character
  * Anything after the initial - will remain
To specify **no** string representation for an enum, create a PR with an implementation of that <br>

Categories are optional, but defining them will provide helper functions to check an enum's category. <br>
If you define a category, make sure to include the ending. If you don't, who knows tbh. <br>
Categories may overlap as much as you'd like.

End the enum definition with !FINISH. <br>

Manually changing the enum / mapping after the fact is risky, as the mapping from enum to string and the
category helper functions both rely on the order and number of enum values.
I'd recommend you keep the input file around as the formal definition of your enu and make changes directly to that.
Then, rerun the program to generate the header.

### Example:


The input file:

```
Vehicles Automotives

!BEGIN POPULAR

!BEGIN CARS 

		RAV4 rav_four
		CAMRY toyota_camry
		MODEL_Y model_y

!END CARS

!BEGIN TRUCKS 
		F_150 -
!END POPULAR

		SIERRA sierra
		SILVERADO silverado

!END TRUCKS

Non_Categorical extra

!FINISH
```

Generates the header Vehicles_generated.hpp:

```c++
#pragma once
#include <unordered_map>
#include <string>
#include <utility>

namespace Automotives {
enum class Vehicles : unsigned {
  RAV4,
  CAMRY,
  MODEL_Y,
  F_150,
  SIERRA,
  SILVERADO,
  Non_Categorical,
};


inline const std::unordered_map<std::string, Vehicles> stringToVehicles{

	{"rav_four", Vehicles::RAV4}, {"toyota_camry", Vehicles::CAMRY}, {"model_y", Vehicles::MODEL_Y}, 
	{"", Vehicles::F_150}, {"sierra", Vehicles::SIERRA}, {"silverado", Vehicles::SILVERADO}, 
	{"extra", Vehicles::Non_Categorical}, 
};


constexpr const char* vehiclesToString(const Vehicles e) {
constexpr const char* toString[] = {

	"rav_four","toyota_camry","model_y",
	"","sierra","silverado",
	"extra",
};
	return toString[std::to_underlying(e)];
}
constexpr bool isCategoryCARS(const Vehicles e) { return std::to_underlying(e) >= 0 && std::to_underlying(e) < 3; }

constexpr bool isCategoryPOPULAR(const Vehicles e) { return std::to_underlying(e) >= 0 && std::to_underlying(e) < 4; }

constexpr bool isCategoryTRUCKS(const Vehicles e) { return std::to_underlying(e) >= 3 && std::to_underlying(e) < 6; }

}; //namespace Automotives
```
