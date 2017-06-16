//
//  SMTPCompiler.cpp
//  Email_test_project
//
//  Created by Richard Bradshaw on 05/06/2017.
//  Copyright © 2017 Richard Bradshaw. All rights reserved.
//  Ignore the copyright do what you like with this code
//

#include "SMTPCompiler.hpp"

#include <fstream>
#include <iostream>

SMTPCompiler::SMTPCompiler(){
    
    res = CURLE_OK;
    recipients = NULL;
    
    upload_ctx.lines_read = 0;
    
    curl = curl_easy_init();
    
    //use SSL
    curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_TRY);
    
    //accept all built in compressions
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
    
    //callback function must be static
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
    curl_easy_setopt(curl, CURLOPT_READDATA, this);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    
    //debug
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
}

SMTPCompiler::~SMTPCompiler(){

}

//public methods
bool SMTPCompiler::isMsgLoaded(){
    return msgLoaded;
}
void SMTPCompiler::setMsgLoaded(bool _msgLoaded){
    msgLoaded = _msgLoaded;
}

string SMTPCompiler::getFrom(){
    return from;
}

string SMTPCompiler::getFrom(){
    return from;
}

void SMTPCompiler::setFrom(string& _from){
    if(checkEmail(_from)){
        from = _from;
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.c_str());
    }
    else
        from = "";
}

string SMTPCompiler::getUsername(){
    return username;
}
void SMTPCompiler::setUsername(string& _username){
    if(checkEmail(_username)){
        username = _username;
        // Set curl
        curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
    }
    else
        username = "";
}
string SMTPCompiler::getPassword(){
    return password;
}
void SMTPCompiler::setPassword(string& _password){
    password = _password;
    // Set curl
    curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());

}
string SMTPCompiler::getServer(){
    return server;
}
void SMTPCompiler::setServer(string& _server){
    server = _server;
    curl_easy_setopt(curl, CURLOPT_URL, server.c_str());
}
vector<string> SMTPCompiler::getDestinations(){
    return destinations;
}

void SMTPCompiler::setDestinations(vector<string> _destinations){
    
    //must have at least one valid destination email address
    if(_destinations.size() > 0){
        for(string s : destinations){
            if(!checkEmail(s)){
                destinations.clear();
                return;
            }
        }
        
        destinations = _destinations;
        
        //curl add all the recipients
        for(string s : destinations)
            recipients = curl_slist_append(recipients, s.c_str());
        
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
    }
}

string SMTPCompiler::getCertificate(){
    return certificate;
}

void SMTPCompiler::setCertificate(string& _certificate){
    certificate = _certificate;
}

string SMTPCompiler::getSubject(){
    return subject;
}

void SMTPCompiler::setSubject(string& _subject){
    subject = _subject;
}

string SMTPCompiler::getBodyText(){
    return bodyText;
}

void SMTPCompiler::setBodyText(string& _body){
    bodyText = _body;
}

void SMTPCompiler::setBodyContentType(string type){
    bodyContent = type;
}

void SMTPCompiler::setBodyEncoding(string encoding){
    bodyEncoding = encoding;
}


vector<string> SMTPCompiler::getAttachmentList(){
    return attachmentList;
}
void SMTPCompiler::setAttachmentList(vector<string> _attachments){
    if(_attachments.size() > 0)
        attachmentList = _attachments;
}

void SMTPCompiler::send(){
    
    string errorMsg = "";
    
    //get the completed message components
    header = getHeader();
    
    body = getBody();
    
    if(attachmentList.size() > 0)
        attachments = getAttachments();

    //check that the message is complete
    while(true){
        if(from.length() < 2){
            errorMsg = "FROM is not valid";
            break;
        }
        if(destinations.size() < 1){
            errorMsg = "DESTINATION is not valid";
            break;
        }
        if(username.length() < 2){
            errorMsg = "USERNAME is not valid";
            break;
        }
        if(password.length() < 2){
            errorMsg = "PASSWORD is not valid";
            break;
        }
        if(server.length() < 2){
            errorMsg = "SERVER is not valid";
            break;
        }
        if(subject.length() < 2){
            errorMsg = "SUBJECT is not valid";
            break;
        }

        if(bodyContent.length() < 2){
            errorMsg = "BODY CONTENT-TYPE is not valid";
            break;
        }
        
        //setup curl
        
        //Send the message
        res = curl_easy_perform(curl);
        
        //Check for errors
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        
        //Free the list of recipients
        curl_slist_free_all(recipients);
        
        //Always cleanup
        curl_easy_cleanup(curl);
        
        //print();
        
        break;

    }
    
    cout << errorMsg << endl;
    
}

vector<string> SMTPCompiler::getHeader(){
    
    vector<string> header;
    header.push_back( "To: " + destinations.at(0) + CRLF);
    header.push_back("From: " + from + CRLF);
    
    //check for CCs
    if(destinations.size() > 1)
    {
        //start at 1 to ignore the first recipient
        for(int i=1; i< destinations.size(); i++){
            header.push_back("Cc: " + destinations.at(i) + CRLF);
        }
    }
    
    //message is in multiple parts of mixed type
    header.push_back("Content-Type: multipart/mixed;" + CRLF);
    header.push_back(" boundary=" + BOUNDARY + CRLF);

    //message id
    //header.push_back("Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@");
    
    //date & time
    time_t t = time(0);   // get time now
    struct tm *now = localtime( & t );
    
    string weekDay[] = {"Mon","Tues","Wed","Thurs","Fri","Sat","Sun"};
    string month[] ={"Jan","Feb","Mar","Apr","May","June","July","Aug","Sept","Oct","Nov","Dec"};
    
    header.push_back("Date: " + weekDay[now->tm_wday] + " " +
                     to_string(now->tm_mday) +  " " +
                     month[now->tm_mon] + " " +
                     to_string(now->tm_year + 1900) +  " " +
                     to_string(now->tm_hour) + ":" +
                     to_string(now->tm_min) + ":" +
                     to_string(now->tm_sec) + " +1100\r\n");
    
    header.push_back("User-Agent: libcurl-agent/1.0\r\n");
    header.push_back("Subject:" + subject + CRLF);
    header.push_back("MIME-Version: 1.0\r\n");
    header.push_back(CRLF);
    
    return header;
    
}

vector<string> SMTPCompiler::getBody(){
    vector<string> body;
    
    body.push_back(bodyText + CRLF);
   
    return body;
    
}

vector<vector<string>> SMTPCompiler::getAttachments(){
    vector<vector<string>> attachments;
    vector<string> attachment;
    
    if(attachmentList.size() > 0){
        
        for(int i=0; i < attachmentList.size(); i++){

            //base 64 converter
            Base64Codec converter;
            
            //read file and put in a buffer
            string fileBuf = converter.readBinaryFile(attachmentList.at(i));
            string B64_string = converter.base64_encode(fileBuf);
            

            //chunk the data string
            const int PACKET_SIZE = 76;
            int inx = 0;
            
            long dataLen = B64_string.length();
            //long dataLen = fileBuf.length();
            
            if(dataLen > PACKET_SIZE){
                while (inx < (dataLen-PACKET_SIZE))
                {
                    string packet = B64_string.substr(inx,PACKET_SIZE);
                    
                    attachment.push_back(packet);
                    
                    inx += PACKET_SIZE;
                }
                //get last bytes
                long bitsRemaining = dataLen - inx;
                if(bitsRemaining > 0){
                    
                    string packet = B64_string.substr(inx,bitsRemaining);

                    attachment.push_back(packet);

                }
                
            }
            else{
                attachment.push_back(B64_string);
            }
            attachment.push_back(CRLF);
            attachments.push_back(attachment);
            
            //clear attachment for reuse
            attachment.clear();
        }
    }
    return attachments;
}

/**
 * This method is used by the payload_source static function to get the message
 * and add the SMTP bits like boundaries and message terminator.
 */
vector<string> SMTPCompiler::getWholeMessage(){
    vector<string> msg;
    
    for(string s : header)
        msg.push_back(s);
    
    //boundary
    msg.push_back(HYPHENS + BOUNDARY + CRLF);

    //set content type for the body
    msg.push_back("Content-Type: text/plain; charset=US-ASCII" + CRLF);
    msg.push_back(CRLF);
    
    //body in plain text
    for(string s : body)
        msg.push_back(s);
    
    //boundary
    msg.push_back(CRLF);
    msg.push_back(HYPHENS + BOUNDARY + CRLF);


    //attachments in base64
    if(attachments.size() > 0){
        
        int count = 0;
        for(vector<string> attachment : attachments){
            
            //set content type for the attachments
            msg.push_back("Content-Type: " + bodyContent + "; name=" + attachmentList.at(count) + CRLF);
            msg.push_back("Content-Disposition: attachment; filename=" + attachmentList.at(count) + CRLF);
            msg.push_back("Content-Transfer-Encoding: " + bodyEncoding + CRLF);
            msg.push_back(CRLF);

            for(string s : attachment)
                msg.push_back(s);
            
            //boundary
            msg.push_back(CRLF);
            msg.push_back(HYPHENS + BOUNDARY + CRLF);
            
            count++;

        }
        
        //final boundary note the extra hyphens after the boundary
        msg.push_back(CRLF);
        msg.push_back(HYPHENS + BOUNDARY + HYPHENS + CRLF);

        
        //terminate the message
        msg.push_back(CRLF + SMTP_TERMINATOR + CRLF);

    }
    return msg;
}

/**
 * Utility to check an email address is properly formed
 */
bool SMTPCompiler::checkEmail(string& _email){
    
    bool emailOK = false;
    
    auto b=_email.begin(), e=_email.end();
    
    if (  (b=find(b, e, '@')) != e &&
        find(b, e, '.') != e )
    
        emailOK = true;

    else
        cout << _email << "is not a valid email address" << endl;
    
    
    return emailOK;
}

void SMTPCompiler::print(){
    cout << "header" << endl;
    vector<string> header;
    header = getHeader();
    for(string h : header)
        cout << h << endl;
    
    cout << "body" << endl;
    vector<string> body;
    body = getBody();
    for(string b : body)
        cout << b << endl;
    
    cout << "attachments" << endl;
    for(vector<string> attach : attachments){
        if(attach.size() > 0)
            for(string a : attach)
                cout << a << endl;
    }
}

