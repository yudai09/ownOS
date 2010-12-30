#include"kcom.h"

class virString{
protected:
  int length;
  char *pstring;
public:
  const char *toChar()const{
    return pstring;
  }
  int size()const{
    return length;
  }
};

template <int N>
class String : public virString{
  char string[N];
 public:
  String(){
    pstring=string;
    length=0;
  }
  String(const char *c){
    pstring=string;
    length=strlen(c)+1;
    if(length<N){
      strncpy(string,c,length);
    }else{
      length=0;
    }
  }
  String(const virString toCopy){
    pstring=string;
    if(toCopy.size()<length){
      length=toCopy.size();
      strncpy(string,toCopy.toChar(),length);
    }else{
      length=0;
    }
  }
  /*
  const String &operator=(const virString rh){
    
  }
  */
  const String &operator=(const char *rh){
    int size=strlen(rh)+1;
    if(size<length){
      strncpy(string,rh,size);
    }
  }
};

