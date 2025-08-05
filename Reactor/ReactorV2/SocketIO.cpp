#include "SocketIO.hpp"


namespace lg{

int SocketIO::readn(char *buf ,int len){
    int left=len;
    char *pbuf=buf;
    while(left>0){
        int ret=recv(_fd,buf,left,0);
        if(ret<0){
            perror("recv");
        }else if(ret==0){
            return len-left;
        }
        left -= ret;
        pbuf += ret;
    }
    return len;
}
int SocketIO::sendn(const char *buf ,int len){
    int left=len;
    const char *pbuf=buf;
    while(left>0){
        int ret=send(_fd,buf,left,0);
        if(ret<0){
            perror("recv");
        }else if(ret==0){
            return len-left;
        }
        left -= ret;
        pbuf += ret;
    }
    return len;
}
int SocketIO::recvPeek(char* buf,int len){
    int ret = recv(_fd,buf,len,MSG_PEEK);
    if(ret==-1){
        perror("SocketIO 39:recv");
    }
    return ret;
}

int SocketIO::readLine(char* buf,int maxLen){
    int left = maxLen-1;
    char *pbuf=buf;
    int total=0;
    while(left>0){
        int ret=recvPeek(buf,left);
        for(int i=0;i<ret;++i){
            if(buf[i]=='\n'){
                int size=i+1;
                ret = readn(pbuf,size);
                total+=ret;
                buf[total-1]='\0';
                return total;
            }
        }
        ret =recv(_fd,buf,left,0);
        left-=ret;
        pbuf+=ret;
        total+=ret;
    }
    buf[maxLen-1]='\0';
    return maxLen-1;
}
}
