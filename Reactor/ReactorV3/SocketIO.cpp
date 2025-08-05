#include "SocketIO.hpp"
namespace lg{

SocketIO::SocketIO(int fd)
:_fd(fd){

}

int SocketIO::recvn(char *buf,int len){
    int left=len;
    char *pbuf=buf;
    while(left>0){
        int ret=recv(_fd,pbuf,left,0);
        if(ret<0){
            perror("SocketIO:recv");
            exit(EXIT_FAILURE);
        }else if(ret==0){
            return len-left;
        }
        left-=ret;
        pbuf+=ret;
    }
    return len;
}

int SocketIO::sendn(const char *buf,int len){
    int left=len;
    const char *pbuf=buf;
    while(left>0){
        int ret=send(_fd,pbuf,left,0);
        if(ret<0){
            perror("SocketIO:send");
            exit(EXIT_FAILURE);
        }
        left-=ret;
        pbuf+=ret;
    }
    return len;
}
int SocketIO::recvPeek(char* buf,int len){
    int ret=recv(_fd,buf,len,MSG_PEEK);
    if(ret<0){
        perror("SocketIO:recvPeek");
        exit(EXIT_FAILURE);
    }
    return ret;
}
int SocketIO::recvLine(char*buf,int maxlen){
    int left=maxlen-1;
    char *pbuf=buf;
    int total=0;
    while(left>0){
        int ret=recvPeek(pbuf,left);
            for(int i=0;i<ret;++i){
                if(pbuf[i]=='\n'){
                    int index=i+1;
                    ret=recvn(pbuf,index);
                    pbuf[i]='\0';
                    total+=ret;
                    return total;
                }
            }
        ret=recvn(pbuf,ret);
        left-=ret;
        pbuf+=ret;
        total+=ret;
    }
    buf[maxlen-1]='\0';
    return maxlen-1;
}



}//end of namespace lg
