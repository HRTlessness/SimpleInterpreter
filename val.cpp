#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <iomanip>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include "val.h"

using namespace std;

Value Value::operator+(const Value& op) const
{
    double op1;
    double op2;
    double ans;

    if (IsString() && op.IsString()) {
        try {
            op1 = stod(GetString());
            op2 = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = op1 + op2; 
        return Value(ans);
    }

    else if (IsString() && op.IsInt()) {
        try {
            op1 = stod(GetString());
            op2 = op.GetInt();
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = op1 + op2; 
        return Value(ans);
    }

    else if (IsInt() && op.IsString()) {
        try {
            op1 = GetInt();
            op2 = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = op1 + op2; 
        return Value(ans);
    }

    else if (IsReal() && op.IsString()) {
        try {
            op1 = GetReal();
            op2 = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = op1 + op2; 
        return Value(ans);
    }

    else if (IsString() && op.IsReal()) {
        try {
            op1 = stod(GetString());
            op2 = op.GetReal();
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = op1 + op2; 
        return Value(ans);
    }

    else if (IsReal() && op.IsReal()) {
        op1 = GetReal();
        op2 = op.GetReal();
        ans = op1 + op2;
        return Value(ans);
    }

    else if (IsReal() && op.IsInt())
    {
        op1 = GetReal();
        op2 = op.GetInt();
        ans = op1 + op2;
        return Value(ans);
    }

    else if (IsInt() && op.IsReal())
    {
        op1 = GetInt();
        op2 = op.GetReal();
        ans = op1 + op2;
        return Value(ans);
    }

    else if (IsInt() && op.IsInt())
    {
        op1 = GetInt();
        op2 = op.GetInt();
        ans = op1 + op2;
        return Value((int)ans);
    }

    else { //ERR
        return Value();
    }
}

Value Value::operator-(const Value& op) const
{
    double op1;
    double op2;
    double ans;

    if (IsString() && op.IsString()) {
        try {
            op1 = stod(GetString());
            op2 = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = op1 - op2; 
        return Value(ans);
    }

    else if (IsString() && op.IsInt()) {
        try {
            op1 = stod(GetString());
            op2 = op.GetInt();
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = op1 - op2; 
        return Value(ans);
    }

    else if (IsInt() && op.IsString()) {
        try {
            op1 = GetInt();
            op2 = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = op1 - op2; 
        return Value(ans);
    }

    else if (IsReal() && op.IsString()) {
        try {
            op1 = GetReal();
            op2 = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = op1 - op2; 
        return Value(ans);
    }

    else if (IsString() && op.IsReal()) {
        try {
            op1 = stod(GetString());
            op2 = op.GetReal();
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = op1 - op2; 
        return Value(ans);
    }

    else if (IsReal() && op.IsReal()) {
        op1 = GetReal();
        op2 = op.GetReal();
        ans = op1 - op2;
        return Value(ans);
    }

    else if (IsReal() && op.IsInt())
    {
        op1 = GetReal();
        op2 = op.GetInt();
        ans = op1 - op2;
        return Value(ans);
    }

    else if (IsInt() && op.IsReal())
    {
        op1 = GetInt();
        op2 = op.GetReal();
        ans = op1 - op2;
        return Value(ans);
    }

    else if (IsInt() && op.IsInt())
    {
        op1 = GetInt();
        op2 = op.GetInt();
        ans = op1 - op2;
        return Value((int)ans);
    }

    else { //ERR
        return Value();
    }
}

Value Value::operator*(const Value& op) const
{
    double op1;
    double op2;
    double ans;

    if (IsString() && op.IsString()) {
        try {
            op1 = stod(GetString());
            op2 = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = op1 * op2; 
        return Value(ans);
    }

    else if (IsString() && op.IsInt()) {
        try {
            op1 = stod(GetString());
            op2 = op.GetInt();
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = op1 * op2; 
        return Value(ans);
    }

    else if (IsInt() && op.IsString()) {
        try {
            op1 = GetInt();
            op2 = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = op1 * op2; 
        return Value(ans);
    }

    else if (IsReal() && op.IsString()) {
        try {
            op1 = GetReal();
            op2 = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = op1 * op2; 
        return Value(ans);
    }

    else if (IsString() && op.IsReal()) {
        try {
            op1 = stod(GetString());
            op2 = op.GetReal();
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = op1 * op2; 
        return Value(ans);
    }

    else if (IsReal() && op.IsReal()) {
        op1 = GetReal();
        op2 = op.GetReal();
        ans = op1 * op2;
        return Value(ans);
    }

    else if (IsReal() && op.IsInt())
    {
        op1 = GetReal();
        op2 = op.GetInt();
        ans = op1 * op2;
        return Value(ans);
    }

    else if (IsInt() && op.IsReal())
    {
        op1 = GetInt();
        op2 = op.GetReal();
        ans = op1 * op2;
        return Value(ans);
    }

    else if (IsInt() && op.IsInt())
    {
        op1 = GetInt();
        op2 = op.GetInt();
        ans = op1 * op2;
        return Value((int)ans);
    }

    else { //ERR
        return Value();
    }
}

Value Value::operator/(const Value& op) const 
{
    double dividend;
    double divisor;
    double ans;

    if (IsString() && op.IsString()) {
        try {
            dividend = stod(GetString());
            divisor = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        if (divisor == 0)
        {
            return Value();
        }
        ans = dividend / divisor; 
        return Value(ans);
    }

    else if (IsString() && op.IsInt()) {
        try {
            dividend = stod(GetString());
            divisor = op.GetInt();
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        if (divisor == 0)
        {
            return Value();
        }
        ans = dividend / divisor; 
        return Value(ans);
    }

    else if (IsInt() && op.IsString()) {
        try {
            dividend = GetInt();
            divisor = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        if (divisor == 0)
        {
            return Value();
        }
        ans = dividend / divisor; 
        return Value(ans);
    }

    else if (IsReal() && op.IsString()) {
        try {
            dividend = GetReal();
            divisor = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        if (divisor == 0)
        {
            return Value();
        }
        ans = dividend / divisor; 
        return Value(ans);
    }

    else if (IsString() && op.IsReal()) {
        try {
            dividend = stod(GetString());
            divisor = op.GetReal();
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        if (divisor == 0)
        {
            return Value();
        }
        ans = dividend / divisor; 
        return Value(ans);
    }

    else if (IsReal() && op.IsReal()) {
        dividend = GetReal();
        divisor = op.GetReal();
        if (divisor == 0)
        {
            return Value();
        }
        ans = dividend / divisor;
        return Value(ans);
    }

    else if (IsReal() && op.IsInt())
    {
        dividend = GetReal();
        divisor = op.GetInt();
        if (divisor == 0)
        {
            return Value();
        }
        ans = dividend / divisor;
        return Value(ans);
    }

    else if (IsInt() && op.IsReal())
    {
        dividend = GetInt();
        divisor = op.GetReal();
        if (divisor == 0)
        {
            return Value();
        }
        ans = dividend / divisor;
        return Value(ans);
    }

    else if (IsInt() && op.IsInt())
    {
        dividend = GetInt();
        divisor = op.GetInt();
        if (divisor == 0)
        {
            return Value();
        }
        ans = dividend / divisor;
        return Value((int)ans);
    }

    else { //ERR
        return Value();
    }
}

Value Value::operator%(const Value& op) const
{
    int dividend;
    int divisor;
    int ans;

    if (IsString() && op.IsString()) {
        try {
            dividend = stod(GetString());
            divisor = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = dividend % divisor; 
        return Value(ans);
    }

    else if (IsInt() && op.IsString()) {
        try {
            dividend = GetInt();
            divisor = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = dividend % divisor; 
        return Value(ans);
    }

    else if (IsString() && op.IsInt()) {
        try {
            dividend = stod(GetString());
            divisor = op.GetInt();
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = dividend % divisor; 
        return Value(ans);
    }

    else if (IsInt() && op.IsInt()) {
        dividend = GetInt();
        divisor = op.GetInt();
        ans = dividend % divisor; 
        return Value(ans);
    }
    else{
        return Value();
    }
}

Value Value::div(const Value& op) const
{
    int dividend;
    int divisor;
    int ans;

    if (IsString() && op.IsString()) {
        try {
            dividend = stod(GetString());
            divisor = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = dividend / divisor; 
        return Value(ans);
    }

    else if (IsInt() && op.IsString()) {
        try {
            dividend = GetInt();
            divisor = stod(op.GetString());
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = dividend / divisor; 
        return Value(ans);
    }

    else if (IsString() && op.IsInt()) {
        try {
            dividend = stod(GetString());
            divisor = op.GetInt();
        }
        catch (invalid_argument & arg) {
            return Value();
        }
        ans = dividend / divisor; 
        return Value(ans);
    }

    else if (IsInt() && op.IsInt()) {
        dividend = GetInt();
        divisor = op.GetInt();
        ans = dividend / divisor; 
        return Value(ans);
    }
    else{
        return Value();
    }
}

Value Value::operator==(const Value& op) const
{
    double op1;
    double op2;
    bool ans;
    if (IsInt() && op.IsInt())
    {
        op1 = GetInt();
        op2 = op.GetInt();
        ans = op1 == op2;
        return Value(ans);
    }

    else if (IsInt() && op.IsReal())
    {
        op1 = GetInt();
        op2 = op.GetReal();
        ans = op1 == op2;
        return Value(ans);
    }

    else if (IsReal() && op.IsInt())
    {
        op1 = GetReal();
        op2 = op.GetInt();
        ans = op1 == op2;
        return Value(ans);
    }

    else if (IsReal() && op.IsReal())
    {
        op1 = GetReal();
        op2 = op.GetReal();
        ans = op1 == op2;
        return Value(ans);
    }

    else if (IsBool() && op.IsBool())
    {
        op1 = GetBool();
        op2 = op.GetBool();
        ans = op1 == op2;
        return Value(ans);
    }

    else if (IsString() && op.IsString())
    {
        ans = GetString() == op.GetString();
        return Value(ans);
    }

    else { //ERR
        return Value();
    }
}

Value Value::operator>(const Value& op) const
{
    double op1;
    double op2;
    bool ans;

    if (IsReal() && op.IsReal())
    {
        op1 = GetReal();
        op2 = op.GetReal();
        ans = op1 > op2;
        return Value(ans);
    }

    else if(IsReal() && op.IsInt())
    {
        op1 = GetReal();
        op2 = op.GetInt();
        ans = op1 > op2;
        return Value(ans);
    }

    else if(IsInt() && op.IsReal())
    {
        op1 = GetInt();
        op2 = op.GetReal();
        ans = op1 > op2;
        return Value(ans);
    }

    else if(IsInt() && op.IsInt())
    {
        op1 = GetInt();
        op2 = op.GetInt();
        ans = op1 > op2;
        return Value(ans);
    }

    else
    {
        return Value();
    }
}

Value Value::operator<(const Value& op) const
{
    double op1;
    double op2;
    bool ans;

    if (IsReal() && op.IsReal())
    {
        op1 = GetReal();
        op2 = op.GetReal();
        ans = op1 < op2;
        return Value(ans);
    }

    else if(IsReal() && op.IsInt())
    {
        op1 = GetReal();
        op2 = op.GetInt();
        ans = op1 < op2;
        return Value(ans);
    }

    else if(IsInt() && op.IsReal())
    {
        op1 = GetInt();
        op2 = op.GetReal();
        ans = op1 < op2;
        return Value(ans);
    }

    else if(IsInt() && op.IsInt())
    {
        op1 = GetInt();
        op2 = op.GetInt();
        ans = op1 < op2;
        return Value(ans);
    }
    
    else
    {
        return Value();
    }
}

Value Value::idiv(const Value& op) const
{
    return Value();
}

Value Value::operator&&(const Value& oper) const
{
    bool op1;
    bool op2;
    bool ans;

    if (IsBool() && oper.IsBool())
    {
        op1 = GetBool();
        op2 = oper.GetBool();
        ans = op1 == op2;
        return Value(ans);
    }

    else { //ERR
        return Value();
    }
}

Value Value::operator||(const Value& op) const
{
    bool op1;
    bool op2;
    bool ans;

    if (IsBool() && op.IsBool())
    {
        op1 = GetBool();
        op2 = op.GetBool();
        ans = op1 || op2;
        return Value(ans);
    }

    else { //ERR
        return Value();
    }
}

Value Value::operator!() const
{
    bool op;
    bool ans;

    if(IsBool())
    {
        op = GetBool();
        ans = !op;
        return Value(ans);
    }
    else
    {
        return Value();
    }
}
