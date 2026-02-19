#pragma once
#include <unordered_map>
#include <string>
#include <utility>

namespace lexer {
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

}; //namespace lexer