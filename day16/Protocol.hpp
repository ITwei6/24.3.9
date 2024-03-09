// // 在网络通信之前，我们服务器端和客户端都需要知道协议。我们也可以自己定制协议，这个协议要被双方都能识别
// // 比如我们可以定制一个计数器协议。协议就是一种约定，除了数据本身还有其他的字段。
// // 1.我们要求将数据以结构化的形式保存这样双方都可以识别这个结构体对象，但传入网络里时，需要转换成字符类型。这个过程就是序列化.序列化的过程就是在构建有效载荷
// // 2.对方接收到字符串类型的数据时，想要用服务操作时，发现是不能操作的，是因为它不认识，这时还需要将字符类型转成结构体类型，这个过程叫做反序列化。
// // 3.为了能让对方接收时，能接收读取到对方想要的完整报文时，我们采取添加报头的形式来解决。
// // 4.所以在将报文传入到网络里时，还需要添加报文，当对端接收到报文时，想要对它进行处理之前，还需要将报文的报头解包才可以正确处理。

// #include <iostream>
// #include <string>

// const std::string blank_space = " ";
// const std::string protocol_space="\n";
// // 封包：报文在发送到网络之前需要添加一些报头，来达到一些要求
// std::string Encode(const std::string &content)//content就是有效载荷
// {
//   //"x + y"------>"len"\n"x + y"\n"   添加了一个报文长度和两个\n
//   std::string packpage=std::to_string(content.size());
//   packpage+=protocol_space;
//   packpage+=content;
//   packpage+=protocol_space;
//   return packpage;
// }

// // 解包：对端读取到报文(可能读取到的不是想要的，根据原先添加上去的报头来获取准确想要的报文)，想要处理它，需要先解除报头才能处理
// bool Decode(const std::string &packpage, std::string *content)
// { 
//   //"len"\n"x + y"\n"---->"x + y"
//   std::size_t pos=packpage.find(protocol_space);
//   if(pos==std::string::npos)
//   return false;

//   std::string len_str=packpage.substr(0,pos);
//   //判断一下是否读取的内容是全部的
//   std::size_t len =std::stoi(len_str);
//   std::size_t total_len=len_str.size()+len+2;
//   if(packpage.size()!=total_len)
//   return false;
//   *content=packpage.substr(pos+1,len); 
// }

// class Request
// {
// public:
//   Request()
//   {}
//   Request(int data1, int data2, char op) : _x(data1), _y(data2), _op(op) // 最初形成结构化数据
//   {
//   }
//   bool Serialize(std::string *out) // 序列化，单纯的就是将结构体转换成字符串
//   {
//     // 构建报文的有效载荷
//     // struct==》"x + y"
//     std::string s = std::to_string(_x);
//     s += blank_space;
//     s += _op;
//     s += blank_space;
//     s += std::to_string(_y);
//     *out = s;
//     return true;
//   }
//   bool Deserialize(std::string &in) // 反序列化，就单纯的将字符串类型转成结构体
//   {
//     //"x + y"==>struct
//     //获取左操作数x
//     std::size_t left=in.find(blank_space);
//     if(left==std::string::npos)
//     return false;
//     std::string part_x=in.substr(0,left);
//     //获取右操作数y
//     std::size_t right=in.rfind(blank_space);
//     if(right==std::string::npos)
//     return false;
//     std::string part_y=in.substr(right+1);
//     //获取操作码op
//     if(left+2!=right)
//     return false;

//     _op=in[left+1];
//     _x=std::stoi(part_x);
//     _y=std::stoi(part_y);
//     return true;
//   }

// public: // x + y
//   int _x;
//   int _y;
//   char _op;
// };
// class Response
// {
// public:
//   Response(int reslut, int code) : _reslut(reslut), _code(code)
//   {
//   }
//   Response()
//   {}
//   bool Serialize(std::string *out) // 序列化，单纯的就是将结构体转换成字符串
//   {
//     //"reslut code"
//     //构建报文的有效载荷
//     std::string s=std::to_string(_reslut);
//     s+=blank_space;
//     s+=std::to_string(_code);
//     *out=s;
//   }
//    bool Deserialize(std::string &in)
//    {
//     //"reslut code"-->结构体类型
//     std::size_t pos=in.find(blank_space);
//     if(pos==std::string::npos)
//     return false;

//     std::string part_left=in.substr(0,pos);
//     std::string part_right=in.substr(pos+1);

//     _reslut=std::stoi(part_left);
//     _code=std::stoi(part_right);
//     return true;
//    }

// public:
//   int _reslut;
//   int _code;
// };
#pragma once

#include <iostream>
#include <string>
const std::string blank_space_sep = " ";
const std::string protocol_sep = "\n";

std::string Encode(std::string &content)
{
    std::string package = std::to_string(content.size());
    package += protocol_sep;
    package += content;
    package += protocol_sep;

    return package;
}

// "len"\n"x op y"\nXXXXXX
// "protocolnumber"\n"len"\n"x op y"\nXXXXXX
bool Decode(std::string &package, std::string *content)
{
    std::size_t pos = package.find(protocol_sep);
    if(pos == std::string::npos) return false;
    std::string len_str = package.substr(0, pos);
    std::size_t len = std::stoi(len_str);
    // package = len_str + content_str + 2
    std::size_t total_len = len_str.size() + len + 2;
    if(package.size() < total_len) return false;

    *content = package.substr(pos+1, len);
    // earse 移除报文 package.erase(0, total_len);
    package.erase(0, total_len);

    return true;
}


// json, protobuf
class Request
{
public:
    Request(int data1, int data2, char oper) : x(data1), y(data2), op(oper)
    {
    }
    Request()
    {}
public:
    bool Serialize(std::string *out)
    {
        // 构建报文的有效载荷
        // struct => string, "x op y"
        std::string s = std::to_string(x);
        s += blank_space_sep;
        s += op;
        s += blank_space_sep;
        s += std::to_string(y);
        *out = s;
        return true;

    }
    bool Deserialize(const std::string &in) // "x op y"
    {

        std::size_t left = in.find(blank_space_sep);
        if (left == std::string::npos)
            return false;
        std::string part_x = in.substr(0, left);

        std::size_t right = in.rfind(blank_space_sep);
        if (right == std::string::npos)
            return false;
        std::string part_y = in.substr(right + 1);

        if (left + 2 != right)
            return false;
        op = in[left + 1];
        x = std::stoi(part_x);
        y = std::stoi(part_y);
        return true;

    }
    void DebugPrint()
    {
        std::cout << "新请求构建完成:  " << x << op << y << "=?" << std::endl;
    }
public:
    // x op y
    int x;
    int y;
    char op; // + - * / %
};

class Response
{
public:
    Response(int res, int c) : result(res), code(c)
    {
    }

    Response()
    {}
public:
    bool Serialize(std::string *out)
    {
        // "result code"
        // 构建报文的有效载荷
        std::string s = std::to_string(result);
        s += blank_space_sep;
        s += std::to_string(code);
        *out = s;
        return true;
    }
    bool Deserialize(const std::string &in) // "result code"
    {

        std::size_t pos = in.find(blank_space_sep);
        if (pos == std::string::npos)
            return false;
        std::string part_left = in.substr(0, pos);
        std::string part_right = in.substr(pos+1);

        result = std::stoi(part_left);
        code = std::stoi(part_right);

        return true;

    }
    void DebugPrint()
    {
        std::cout << "结果响应完成, result: " << result << ", code: "<< code << std::endl;
    }
public:
    int result;
    int code; // 0，可信，否则!0具体是几，表明对应的错误原因
};