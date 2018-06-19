/*
 * registersIPBUS.cpp
 *
 *  Created on: Jun 26, 2015
 *      Author: stefano
 */

#include <registerIPBUS.h>
#include <stdio.h>

registersIPBUS::registersIPBUS(hwIntWr *hw, const char *reg_id, uint8_t d):
		 _hw(hw),_reg_id(reg_id), _d(d) {
	     _base_address = _hw->getNode(reg_id).getAddress();
		 }

void registersIPBUS::set(uint32_t address, uint32_t value)
{
	std::string s = this->fromAddrtoNode(_base_address + address);
	//printf("set %s  [%08x]-- > %08x\n", s, _base_address + address,  value);
	if (!s.empty()){
		_hw->getNode(_reg_id).getNode(s).write(value);
		if(_d) _hw->dispatch();
	}
	return;
}

uint32_t registersIPBUS::get(uint32_t address)
{
	std::string s = this->fromAddrtoNode(_base_address + address);
	if (!s.empty()){
		ValWord< uint32_t > v = _hw->getNode(_reg_id).getNode(s).read();
		if(_d) {
			_hw->dispatch();
			//printf("get %s  [%08x]-- > %08x\n", c, _base_address + address,  v.value());
			return v.value();

		}
	}
	return 0;
}


//void registecdcvfrsIPBUS::set(uint32_t address, uint32_t* values, uint32_t block_size)
//{
//	const char * c = this->fromAddrtoNode(_base_address + address);
//	if (c){
//		  std::vector<uint32_t> lValues;
//		  for(size_t i=0; i< block_size; i++)
//		  	lValues.push_back(values[i]);
//          _hw->getNode(_reg_id).getNode(c).writeBlock(lValues);
//		  if(_d)
//			  _hw->dispatch();
//	}
//	return;
//}

uint32_t registersIPBUS::get(uint32_t address, uint32_t* block, uint32_t block_size )
{
	std::string s = this->fromAddrtoNode(_base_address + address);
	if (!s.empty()){
		ValVector< uint32_t > v = _hw->getNode(_reg_id).getNode(s).readBlock(block_size);
		if(_d) {
			_hw->dispatch();
			size_t i = 0;
			for (ValVector<uint32_t>::const_iterator lIt = v.begin(); lIt != v.end(); lIt++)
			    block[i++] = *lIt;
		}
	}
	return 0;
}

std::string registersIPBUS::fromAddrtoNode(uint32_t id){
	std::string s = this->isAddressInMap(id);
	if (s.empty())
		s = this->instertInMap(id);
	if (s.empty())
		printf("Address [%08x] is not valid\n", id);
	return s;
}

std::string registersIPBUS::isAddressInMap(uint32_t id){


	if (_m.empty())
		return std::string("");

	mT::iterator  it= _m.find(id);
	if(it != _m.end())
		return it->second;

	return std::string("");

}

std::string  registersIPBUS::instertInMap(uint32_t id){
	std::vector<std::string> v =  _hw->getNode(_reg_id).getNodes();
	for(auto s : v){
		uint32_t a = _hw->getNode(_reg_id).getNode(s).getAddress();
		if (a == id){
			//const char * c= s.c_str();
		    _m.emplace(a, s);
		    return s;
		}
	}
	return "";
}

extern "C" {

registersIPBUS *newregIP(void *h, void *r){
	return new registersIPBUS((hwIntWr *) h,( const char *)r);
}

};
