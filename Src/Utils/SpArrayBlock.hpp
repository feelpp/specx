#ifndef SPARRAYBLOCK_HPP
#define SPARRAYBLOCK_HPP

#include <type_traits>

#include "SpAlignment.hpp"
#include "Config/SpConfig.hpp"

template <typename DataType, class AlignmentType=SpAlignment<alignof(DataType)>, std::enable_if_t<is_instantiation_of_sp_alignment<AlignmentType>::value, int> = 0>
class SpArrayBlock {
	static_assert(std::is_trivially_copyable_v<DataType>);
	static_assert(AlignmentType::value >= alignof(DataType));
	
private:
	DataType* data;
	std::size_t nbElts;
	 
public:
	using value_type = DataType;
	
	SPHOST SPDEVICE
	SpArrayBlock() : data(nullptr), nbElts(0) {}
	
	SPHOST SPDEVICE
	SpArrayBlock(void* inData, std::size_t inNbElts) :data(inData), nbElts(inNbElts) {}
	
	static constexpr auto getAlignment() {
		return static_cast<std::size_t>(AlignmentType::value);
	}
	
	static constexpr std::size_t getSize(std::size_t nbElts, std::size_t inAlignment) {
		return (nbElts * sizeof(DataType) + inAlignment - 1) & ~(inAlignment - 1);
	}
	
	SPHOST SPDEVICE
	auto begin() {
		return data;
	}
	
	SPHOST SPDEVICE
	auto end() {
		return data + nbElts;
	}
};

#endif
