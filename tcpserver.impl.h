//public
tcpServer::tcpServer(uint16_t port)
: _server(port)
,_currentTotalStreams(0)
{
}

void tcpServer::begin(){
    _server.begin();
}

void tcpServer::close(){
    _server.close();
}

void tcpServer::stop(){
    close();
}

String tcpServer::getCurrentTotalStreams(){
    return (String)_currentTotalStreams;
}

void tcpServer::handleClient(){
    manageStream();
    if(_currentTotalStreams < 1){
        return;
    }
    checkMessageFromStream();
    yield();
}
//private
void tcpServer::manageStream(){
    if(_server.hasClient()){
        blnprint("[TCP_SERVER] Has client connect... ");
        if(!setClientToStream()){
            WiFiClient serverClient = _server.available();
            serverClient.stop();
            bprint("Failse");
        }
    }
}

bool tcpServer::setClientToStream(){
    for(uint8_t i = 0; i < MAX_SRV_STREAMS; i++){
        if(!_Stream[i].status){
            _Stream[i].client = _server.available();
            _Stream[i].lastReceveMessageTime = millis();
            _Stream[i].status = true;
            _Stream[i].id = i;
            _currentTotalStreams++;
            bprint("Set client to stream " +  (String)i);
            return 1;
        }
    }
    return 0;
}

void tcpServer::checkMessageFromStream(){
    for(int8_t i = 0, remain = _currentTotalStreams; i < MAX_SRV_STREAMS && remain > 0; i++){
        if(_Stream[i].status){
            remain--;
            if(_Stream[i].client.available() > 0){
                getMessage(&_Stream[i]);
                _Stream[i].lastReceveMessageTime = millis();
            }else{
                if(millis() - _Stream[i].lastReceveMessageTime > TIME_OUT_STREAM){
                    _Stream[i].client.stop();
                    _Stream[i].status = false;
                    _currentTotalStreams--;
                    blnprint("[TCP_SERVER] Close Stream " + (String)i + " . Time out Stream!!!");
                }
            }
        }
    }
}

void tcpServer::getMessage(connectionStream *Stream){
    while(Stream->client.available() > 0){
        Stream->command[Stream->currentAddress] = Stream->client.read();
        Stream->currentAddress++;
    }
    if(Stream->currentAddress > 4){
        if(Stream->command[0] != BYTE_COMMAND_CLIENT){
            blnprintf("[TCP_SERVER] >> Command : %d => Failse struct", Stream->command[0]);
            Stream->currentAddress = 0;
            return;
        }
        uint16_t len = Stream->command[3]*255 + Stream->command[4];
        if(Stream->currentAddress - (len + 5) >= 0){
            blnprintf("[TCP_SERVER] Stream %d >> [%d]:", Stream->id, Stream->command[0]);
            bprintf("[%d%d]:", Stream->command[1],Stream->command[2]);
            bprintf("[%05d]:[", len);
            for(uint16_t i = 5; i < 5 + len; i++)
                bprint((String)(char)Stream->command[i]);
            bprint("]");
            Stream->currentAddress = 0;
            cmd(Stream, len);
        }
    }
}

void tcpServer::sendMessageToClient(connectionStream *Stream, uint8_t messageId2){
    String buf = "";
    sendMessageToClient(Stream,0,messageId2, &buf);
}

void tcpServer::sendMessageToClient(connectionStream *Stream,uint8_t messageId2, String *body){
    sendMessageToClient(Stream,0,messageId2, body);
}

void tcpServer::sendMessageToClient(connectionStream *Stream, uint8_t messageId1, uint8_t messageId2, String *body){
    uint16_t length = body->length();
    Stream->client.write(BYTE_COMMAND_SERVER);
    Stream->client.write(messageId1);
    Stream->client.write(messageId2);
    if(length == 0){
        Stream->client.write((uint8_t)0);
        Stream->client.write((uint8_t)0);
    }else if(length < 256){
        Stream->client.write((uint8_t)0);
        Stream->client.write((uint8_t)length);
        Stream->client.write(body->c_str());
    }else{
        Stream->client.write((uint8_t)length/255);
        Stream->client.write((uint8_t)length%255);
        Stream->client.write(body->c_str());
    }
    blnprintf("[TCP_SERVER] Stream %d << [%d]:", Stream->id,BYTE_COMMAND_SERVER);
    bprintf("[%d%d]:[%d%d]:[",messageId1,messageId2,length/255,length%255);
    bprint(body);
    bprint("]");
}

String tcpServer::decodeString(connectionStream *Stream,uint16_t length, String name){
    name += "=";
    char charName[name.length()];
    name.toCharArray(charName, name.length() + 1);

    for(uint16_t i = 5;i<length + 5; i++){
        for(uint8_t j=0; j < name.length();j++){
            if(Stream->command[i + j] != (uint8_t)charName[j])
                break;
            if(j == name.length() - 1){
                String result = "";
                for(uint16_t k = i+j+1; k < length + 5; k++){
                    if(Stream->command[k] == 38)
                        break;
                    result += char(Stream->command[k]);
                }
                return result;
            }
        }
    }
    return "";
}

void tcpServer::cmd(connectionStream *Stream, uint16_t length){
    #if 0
        ESPcmd messageID = (ESPcmd)Stream->command[1]*255 + Stream->command[2];
    #else
        ESPcmd messageID = (ESPcmd)Stream->command[2];
    #endif
    #if 1
    switch(messageID){
        case SET_PINMODE :{
            uint8_t pin  = decodeString(Stream,length,"pin").toInt();
            uint8_t mode = decodeString(Stream,length,"mode").toInt();
            if(mode == 1)
                pinMode(pin,OUTPUT);
            else if(mode == 2)
                pinMode(pin,INPUT);
            else if(mode == 3)
                pinMode(pin,INPUT_PULLUP);
            sendMessageToClient(Stream,200);
            break;
        }
        case GET_PINMODE :{
            break;
        }
        case SET_DIGITAL:{
            uint8_t pin = decodeString(Stream,length, "pin").toInt();
            uint8_t value = decodeString(Stream,length, "value").toInt();
            digitalWrite(pin,value);
            sendMessageToClient(Stream,200);
            break;
        }
        case GET_DIGITAL:{
            uint8_t pin = decodeString(Stream,length, "pin").toInt();
            String value = (String)digitalRead(pin);
            sendMessageToClient(Stream,200,&value);
            break;
        }
        case SET_ANALOG:{
            uint8_t pin   = decodeString(Stream,length, "pin").toInt();
            uint8_t value = decodeString(Stream,length, "value").toInt();
            analogWrite(pin, value);
            sendMessageToClient(Stream,200);
            break;
        }
        case GET_ANALOG:{
            String value = (String)analogRead(A0);            
            sendMessageToClient(Stream,200,&value);
            break;
        }
        case RETURN_CONTENT_READ :{
            String str = "";
            for(uint16_t i=5; i < length+5; i++)
                str += Stream->command[i];
            sendMessageToClient(Stream,200, &str);
            break;
        }
        case NOTIFY_SUCCSESSFUL:{
            sendMessageToClient(Stream,200);
            break;
        }
        case NOTIFY_PING:{
            sendMessageToClient(Stream,200);
            break;
        }
        default:{
            break;
        }
    }
    #endif
}









