#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <stdexcept>
#include <string>

namespace battleship
{

    class BattleshipLogicError : public std::logic_error
    {
    public:
        explicit BattleshipLogicError(const char* what_arg);
        explicit BattleshipLogicError(const std::string& what_arg);
    };

    class BattleshipRuntimeError : public std::runtime_error
    {
    public:
        explicit BattleshipRuntimeError(const char* what_arg);
        explicit BattleshipRuntimeError(const std::string& what_arg);
    };

    class InvalidCoordinateError : public BattleshipRuntimeError
    {
    public:
        explicit InvalidCoordinateError(const char* what_arg);
        explicit InvalidCoordinateError(const std::string& what_arg);
    };

    class InvalidShipLocationError : public BattleshipRuntimeError
    {
    public:
        explicit InvalidShipLocationError(const char* what_arg);
        explicit InvalidShipLocationError(const std::string& what_arg);
    };

    class ArgumentsError : public BattleshipRuntimeError
    {
    public:
        explicit ArgumentsError(const char* what_arg);
        explicit ArgumentsError(const std::string& what_arg);
    };

}

#endif // !EXCEPTIONS_H_
