#include "simple_parser.hpp"

#include <algorithm>

uint8_t getchecksum(std::vector<uint8_t>::const_iterator begin, std::vector<uint8_t>::const_iterator end)
{
    int whole_checksum = 0;
    uint8_t checksum;

    std::for_each(begin, end, [&whole_checksum, &checksum] (uint8_t n) {
        whole_checksum = whole_checksum + n;
        checksum = ~(0xFF & whole_checksum) + 1;
    });

    return checksum;
}   


void SimpleParser::push(uint8_t b){

    uint8_t check = getchecksum(inputData.cbegin(), inputData.cend());  //контр. суммa для проверки

    if(start)                      
        inputData.push_back(b);     //если старт, принимает данные

    if(check == b && start){        //если контролльная сумма совпала, запускает парсинг пакета
        this->parsing();        
    }

    if(b == 0x53){
        start = true;               //отлавливает стартовый битб, ставид флаг старт
        inputData.push_back(b);
    }


          
}

void SimpleParser::parsing(){           //парсит пакет и создает объекты команд

    if(inputData.empty())
        return;

    if(inputData.at(5) == 0x60){
        POLL poll(inputData);
        packet = std::make_shared<POLL>(poll);
    }

    if(inputData.at(5) == 0x6A){
        BUZ buz(inputData);
        packet = std::make_shared<BUZ>(buz);
    }

}

void SimpleParser::reset(){
    comand.clear();
    inputData.clear();
    packet = nullptr;
    start = false;   
}


std::shared_ptr<Packet> SimpleParser::get_packet() const {

    return packet;
}

std::string POLL::to_string()const{

    return "type:" + this->_type + ",addr:" + std::to_string((int)this->_data.at(1)) +
            ",sqn:" + std::to_string((int)this->_data.at(4));

}

std::string BUZ::to_string()const{

    return "type:" + this->_type + ",addr:" + std::to_string((int)this->_data.at(1)) +
            ",sqn:" + std::to_string((int)this->_data.at(4)) + 
            ",reader:" + std::to_string((int)this->_data.at(6)) +
            ",tone:" + std::to_string((int)this->_data.at(7)) +
            ",on:" + std::to_string((int)this->_data.at(8)) +
            ",off:" + std::to_string((int)this->_data.at(9)) +
            ",count:" + std::to_string((int)this->_data.at(10));
}
