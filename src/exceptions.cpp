#include "exceptions.h"

battleship::BattleshipLogicError::BattleshipLogicError(const char* what_arg)
    : logic_error(what_arg)
{ }

battleship::BattleshipLogicError::BattleshipLogicError(const std::string& what_arg)
    : logic_error(what_arg)
{ }

battleship::BattleshipRuntimeError::BattleshipRuntimeError(const char* what_arg)
    : runtime_error(what_arg)
{ }

battleship::BattleshipRuntimeError::BattleshipRuntimeError(const std::string& what_arg)
    : runtime_error(what_arg)
{ }

battleship::InvalidCoordinateError::InvalidCoordinateError(const char* what_arg)
    : BattleshipRuntimeError(what_arg)
{ }

battleship::InvalidCoordinateError::InvalidCoordinateError(const std::string& what_arg)
    : BattleshipRuntimeError(what_arg)
{ }

battleship::InvalidShipLocationError::InvalidShipLocationError(const char* what_arg)
    : BattleshipRuntimeError(what_arg)
{ }

battleship::InvalidShipLocationError::InvalidShipLocationError(const std::string& what_arg)
    : BattleshipRuntimeError(what_arg)
{ }

battleship::ArgumentsError::ArgumentsError(const char* what_arg)
    : BattleshipRuntimeError(what_arg)
{ }

battleship::ArgumentsError::ArgumentsError(const std::string& what_arg)
    : BattleshipRuntimeError(what_arg)
{ }
