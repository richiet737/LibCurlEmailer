//
//  Base64Codec.cpp
//  Email_test_project
//
//  Created by Richard Bradshaw on 03/06/2017.
//  Copyright © 2017 Richard Bradshaw. All rights reserved.
//  Ignore the copyright do what you like with this code
//

#include "Base64Codec.hpp"

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

using namespace std;

string Base64Codec::readTextFile (const string& path )
{
    std::ostringstream contents;
    std::ifstream inFile(path.c_str());
    contents << inFile.rdbuf();
    
    inFile.close();
    
    return (contents.str());
}

string Base64Codec::readBinaryFile (const string& path )
{
    std::ostringstream contents;
    std::ifstream inFile(path.c_str(), ios::binary);
    contents << inFile.rdbuf();
    
    inFile.close();
    
    return (contents.str());
}

void Base64Codec::writeBinaryFile ( const std::string& path, string data ){
    std::ofstream outFile(path.c_str(), ios::binary);
    //outFile.open(path);
    outFile << data;
    outFile.close();
    
    
}

void Base64Codec::writeTextFile ( const std::string& path, string data){
    std::ofstream outFile(path.c_str());
    outFile.open(path);
    outFile << data;
    outFile.close();
}

string Base64Codec::base64_encode (const string& contents)
{
    const char * data =
    reinterpret_cast<const char*>(contents.data());
    string b64Str = (b64encode(data, contents.size()));
    
    //debug write back as a text file and a rebuilt pdf
    ofstream output1 ("output.txt");
    output1 << b64Str;

    ofstream output2 ("output.pdf");
    string decoded = b64decode(b64Str);
    output2 << decoded;
    
    output1.close();
    output2.close();
    
    return b64Str;
}

string Base64Codec::b64encode( const void* data, size_t in_len)
{
    std::string ret;
    int i = 0;
    int j = 0;
    char char_array_3[3];
    char char_array_4[4];
    char* dataChar = (char*) data;
    
    while (in_len--) {
        char_array_3[i++] = *(dataChar++);

        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            
            i = 0;
        }
    }
    
    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';
        
        char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] =   char_array_3[2] & 0x3f;
        
        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];
        
        while((i++ < 3))
            ret += '=';
        
    }
    
    return ret;
}

string Base64Codec::b64decode(string const& encoded_string)
{
    long in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;
    
    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            
            char_array_3[0] = ( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) +   char_array_4[3];
            
            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;
        
        for (j = 0; j <4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    
    return ret;
}
