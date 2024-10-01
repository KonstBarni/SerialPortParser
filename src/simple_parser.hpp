#ifndef SIMPLE_PARSER_HPP
#define SIMPLE_PARSER_HPP

#include <cstdint>
#include <memory>
#include <vector>
#include <string>

class Packet
{
public:
    virtual std::string to_string() const = 0;
};

class POLL : virtual public Packet              //класс комонды типа POLL 
{
    const std::string _type = "POLL";
    std::vector<uint8_t> _data;
    
public:

    POLL(std::vector<uint8_t>& data) : _data(data){}
    std::string to_string() const override;
    ~POLL(){}
};

class BUZ : virtual public Packet               // класс команды типа BUZ
{
    const std::string _type ="BUZ";
    std::vector<uint8_t> _data;

public:
    BUZ(std::vector<uint8_t>& data): _data(data){}
    std::string to_string() const override;
    ~BUZ(){}
};


class SimpleParser
{
public:
    SimpleParser(){}

    void push(uint8_t b);
    void reset();
    void parsing();

    std::shared_ptr<Packet> get_packet() const;

private:

    bool start = false;                         //флаг старта записи пакета
    std::vector<Packet>comand{};                //вектор класса соманд
    std::vector<uint8_t>inputData{};            //вектор входящих данных
    std::shared_ptr<Packet>packet = nullptr;    //указатель на объект команды
};

#endif // SIMPLE_PARSER_HPP