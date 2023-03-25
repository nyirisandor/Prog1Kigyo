#include <iostream>
#pragma once
template <typename T>
class Matrix
{
protected:
    int colCount,rowCount;
    T* dataArray;
public:
    Matrix(){};
    Matrix(int cols, int rows, T defaultValue = 0){
        this->dataArray = new T[rows*cols];
        this-> colCount = cols;
        this-> rowCount = rows;
        this->reset(defaultValue);
        
    }
    ~Matrix(){
        delete[] this->dataArray;
    }

    void reset(T val){
        for (size_t i = 0; i < this->colCount*this->rowCount; i++)
        {
            this->dataArray[i] = val;
        }
    }

    int Cols(){
        return this->colCount;
    }

    int Rows(){
        return this->rowCount;
    }

    T& at(int i, int j){
        if(i >= this->colCount || j >= this->rowCount){
            throw std::out_of_range("Out of range");
        }

        return(this->dataArray[i*(colCount)+j]);
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& o,Matrix<T>& m){
    for (size_t i = 0; i < m.Rows(); i++)
    {
        for (size_t j = 0; j < m.Cols(); j++)
        {
            o << m.at(i,j) << "";
        }
        o << std::endl;
    }

    return o;
}

