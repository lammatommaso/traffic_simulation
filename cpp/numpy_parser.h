#ifndef NUMPY_PARSER_H
#define NUMPY_PARSER_H

#include<vector>
#include<string>

namespace Numpy_Parser 
{
    inline std::string numpy();
    template<class T> std::string print_array(std::string varname, std::vector<T> array);
    template<class T> std::string print_matrix(std::string varname, std::vector<T> matrix, short n_coloumns);
    template<class T> std::string print_var(std::string varname, T var);
}
#include"numpy_parser.cpp"
#endif