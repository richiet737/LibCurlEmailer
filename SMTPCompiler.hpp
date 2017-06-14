//
//  SMTPCompiler.hpp
//  Email_test_project
//
//  Created by Richard Bradshaw on 05/06/2017.
//  Copyright © 2017 Richard Bradshaw. All rights reserved.
//  Ignore the copyright do what you like with this code
//

#ifndef SMTPCompiler_hpp
#define SMTPCompiler_hpp

#include <stdio.h>
#include <iostream>
#include <curl/curl.h>
#include <vector>
#include "Base64Codec.hpp"


using namespace std;

/**
 * This class builds an SMTP message with attachments and sends them as emails
 * See main.cpp for a working example of this.
 * NOTE: you will need to include libcurl.dylib in your project
 *
 * The method payload_source must be static to work with curl and is therefore in the header file. 
 * You get the implementation data by passing the this pointer into that method see 
 * SMTPCompiler::SMTPCompiler()
 */

class SMTPCompiler{
private:
    string from;
    vector<string> destinations;
    string username;
    string password;
    string server;
    string certificate;
    string subject;
    string bodyText;
    vector<string> attachmentList;
    string bodyContent;
    string bodyEncoding;
    string attachEncoding;
    
    vector<string> header;
    vector<string> body;
    vector<vector<string>> attachments;
    vector<string> getHeader();
    vector<string> getBody();
    vector<vector<string>> getAttachments();
    vector<string> getWholeMessage();
    
    //constants
    //boundary can be any random string
    const string BOUNDARY = "030203080101020302070708";
    const string HYPHENS = "--";
    const string CRLF = "\r\n";
    const char SMTP_TERMINATOR = '.';
    
    //debug
    char *aBuffer;
    
    bool checkEmail(string& _email);
    
    CURL *curl;
    //CURLM *mcurl;
    
    CURLcode res;
    struct curl_slist *recipients;
    
    struct upload_status {
        int lines_read;
    };
    
    struct upload_status upload_ctx;
    
public:
    SMTPCompiler();
    ~SMTPCompiler();
    string getFrom();
    void setFrom(string& _from);
    string getUsername();
    void setUsername(string& _username);
    string getPassword();
    void setPassword(string& _password);
    string getServer();
    void setServer(string& _server);
    vector<string> getDestinations();
    void setDestinations(vector<string> _destinations);
    string getCertificate();
    void setCertificate(string& _certificate);
    string getSubject();
    void setSubject(string& _subject);
    string getBodyText();
    void setBodyText(string& _body);
    void setBodyContentType(string type);
    void setBodyEncoding(string encoding);

    
    //attachment file names
    vector<string> getAttachmentList();
    void setAttachmentList(vector<string> _attachments);
    
    void send();
    
    void print();
    
    
    
    /*
     * static callback functions
     * this is used by curl to send the message
     */
    static size_t payload_source(void *ptr, size_t size, size_t nmemb, void *userp) {
        
        //userp is a void* to your data source in this case the implimentation of this class
        //this is the link between static and implemented
        SMTPCompiler *smtpCompiler = (SMTPCompiler*) userp;
        
        static vector<string> msgToSend;

        if(msgToSend.size() ==0)
            msgToSend = smtpCompiler->getWholeMessage();
        
        const char *data;
        const string newLine = "\n";
        
        if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) {
            
            return 0;
            
        }
        
        //upload_ctx->lines_read is the number of lines read from the prepared message
        if(smtpCompiler->upload_ctx.lines_read < msgToSend.size()){
            data = msgToSend.at(smtpCompiler->upload_ctx.lines_read).c_str();
            
            if(data) {
                                
                size_t len = strlen(data);
                
                //deal with null string. Might not be necessary
                if(len == 0){
                    data = newLine.c_str();
                    len = newLine.length();
                }
                
                memcpy(ptr, data, len);
                smtpCompiler->upload_ctx.lines_read++;
                
                return len;
                
            }
        }
        return 0;
    }    
};

#endif /* SMTPCompiler_hpp */
