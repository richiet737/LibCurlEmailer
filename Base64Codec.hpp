//
//  Base64Codec.hpp
//  Email_test_project
//
//  Created by Richard Bradshaw on 03/06/2017.
//  Copyright © 2017 Richard Bradshaw. All rights reserved.
//  Ignore the copyright do what you like with this code
//

#ifndef Base64Codec_hpp
#define Base64Codec_hpp

#include <stdio.h>
#include <iostream>

using namespace std;

/**
 * This class is used to convert a C++ string to base 64.
 * It is required if you wish to send binary attachments
 * for example PDF files.
 */

class Base64Codec {
private:
    string b64encode( const void* data, size_t in_len);

public:
    string readBinaryFile ( const std::string& path );
    string readTextFile ( const std::string& path );
    void writeBinaryFile ( const std::string& path, string data );
    void writeTextFile ( const std::string& path, string data );
    string base64_encode ( const std::string& contents );

    string b64decode(string const& encoded_string);
};

#endif /* Base64Codec_hpp */
