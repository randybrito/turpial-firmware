/**
 * @file SerializablePOD.h
 * @author Locha Mesh project developers (locha.io)
 * @brief 
 * @version 0.1.1
 * @date 2019-11-02
 * @copyright Copyright (c) 2019 Locha Mesh project developers
 * @license Apache 2.0, see LICENSE file for details
 */

#include <iostream>
#include <cstring>
#include <cstdio>

/**
 * @brief This class is a static one to be used inside data transfer objects to reimplement 
 * the functionality inside DTO methods to serialized and deserialize the information inside
 * 
 * @tparam POD 
 */
template <typename POD>
class SerializablePOD
{
public:
    /**
     * @brief 
     * 
     * @param any basic data type, for instance int, char, bool, float, double 
     * @return size_t 
     */
    static std::size_t serialize_size(POD str)
    {
        return sizeof(str);
    }

    /**
     * @brief 
     * 
     * @param target target is a buffer to store the serialized data
     * @param value  value could be any int,char,bool,float,double
     * @return char* this return the next available address in the buffer
     */
    static char *serialize(char *target, POD value)
    {
        memcpy(target, &value, serialize_size(value));
        return target + sizeof(size_t);
    }

    /**
     * @brief 
     * 
     * @param source //source is a buffer with serialized data to be deserialized
     * @param target is the struct field address to interpolate the data
     * @return const char* this return the address to the next available data space in the buffer
     */
    static const char *deserialize(const char *source, POD &target)
    {
        std::cout << "Vamos a deserializar" << std::endl;
        std::memcpy(&target, source, serialize_size(target));
        return source + sizeof(target);
    }
};

/**
* @brief
* to be able to serialized any char* data we need to store the length of the string and the string itself, 
* to be able to deserilized it
* @param str pointer to char to know the length of the string
* @return size_t this function return the length of the string plus 8 bytes to store the length of the string in serialized buffer 

*/

template <>
std::size_t SerializablePOD<char *>::serialize_size(char *str)
{
    std::cout << "el TAMANO del char* " << sizeof(std::size_t) + std::strlen(str) << std::endl;
    return sizeof(std::size_t) + std::strlen(str);
}

/**
 * @brief 
 * deserializes a dynamically sized char string  (char*) that's null
 * terminated. It returns a pointer trough the `target` argument to the buffer
 * and you need to take ownership of it and `free()` it when necessary to
 * evitate memory leaks.
 * @tparam  
 * @param source  this is the buffer with serialized data to be deserialized 
 * @param target  this is the structure field address to be interpolate with the deserialized data field
 * @return const char* address of the next data inside buffer with serialized data
 */
template<>
const char* SerializablePOD<char*>::deserialize( const char* source, char*& target )
{
    std::size_t length;
    std::cout <<"Vamos a deserializar" << std::endl;
    std::cout <<":----------address of char field in structure------------------------------->"<< static_cast<const void*>(&target)<<std::endl;

    memcpy( &length, source, sizeof(size_t)); //Getting length
    std::cout << "el length of string.."<< (size_t)length << std::endl;
    source = source +sizeof(size_t);

    target = (char*)malloc((length + 1) * sizeof(char)); 
    memcpy( target, source, length);
    target[length] = '\0';


    std::cout << "salida: " << std::string(source,length) << std::endl;
    std::cout << "dato en el field despues de deserializar: : " << std::string(target,length) << std::endl;
    std::cout <<":----------address->"<< static_cast<const void*>(target)<<std::endl;

    return source + length;  

}


/**
 * @brief 
 * 
 * @tparam  
 * @param target this is the buffer to store serialized data 
 * @param value  data to be serialized 
 * @return char* address of the next available address to store the new serialized data inside the buffer
 */

template<>
char* SerializablePOD<char*>::serialize( char* target, char* value )
{  
    
    std::size_t l = strlen(value);
    
    std::cout << ": dir buffer ---------->" << static_cast<const void*> (&target) << std::endl;
    std::cout << ": dir inside pointer to put data ---------->" << static_cast<const void*> (value) << std::endl;
    std::cout << "vamos a serializar char*->" << l << std::endl;
    
    std::memcpy( target, &l , sizeof(size_t) );
    target = target + sizeof(size_t);
    std::memcpy( target, value, std::strlen(value) );
    
    std::cout << std::string(target,l) << std::endl;
    return target + l;

} 

