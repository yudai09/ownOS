#ifndef UTILCLASS_HPP
#define UTILCLASS_HPP

#include"kcom.h"

// template <class T>
// class sized_buffer{
//   size_t size;
//   T *buffer;
// private:
// public:
//   sized_buffer(size_t size){
//     this->size = size;
//     buffer = new T[size];
//   }
// };
class kstring{
private:
  char *pstring;
  int len;
  void copy(const kstring &copied){
    len = copied.length();
    pstring = new char[len+1+1];
    strncpy(pstring,copied.to_char(),len);
  }
public:
  kstring(){
    len = 0;
    pstring=NULL;
  }
  kstring(const char *str){
    len =   strlen(str);
    pstring = new char[len+1];
    strcpy(pstring,str);
  }
  kstring(const char *str,int len){
    pstring = new char[len+1];
    this->len = len;
    strncpy(pstring,str,len);
    pstring[len]='\0';
  }
  kstring(const kstring &copied){
    copy(copied);
  }
  ~kstring(){
    if(pstring!=NULL){
      delete[] pstring;
    }
  }
  int length ()const{
    return len;
  }
  const char *to_char()const{//dangerous
    return pstring;
  }
  const kstring& operator=(const kstring &copied){
    if(pstring != NULL)
      delete []pstring;
    copy(copied);
    return *this;
  }
  const kstring& operator=(const char *copied){
    if(pstring != NULL)
      delete []pstring;

    int slen = strlen(copied);
    len = slen;
    pstring = new char[len+1];
    strcpy(pstring,copied);
    return *this;
  }
  bool operator==(const kstring &rh){
    if(rh.length()!=len){
      return false;
    }
    return strcmp(pstring,rh.to_char(),len)==0;
  }
  bool operator==(const char *str){
    return strncmp(pstring,str,len)==0;
  }


};



template <class TYPE>
class kvector{
private:
  TYPE *elems;
  size_t max_elem;//最大容量
  size_t num_elem;//実際の要素数
  bool copy(const kvector<TYPE> &copied){
    //    kprintf("copy()\n");
    this->max_elem = copied.max_elem;
    this->num_elem = copied.num_elem;
    elems = new TYPE[max_elem];
    for(int i=0;i<max_elem;i++){
      elems[i]=copied[i];
    }
    return true;
  }
public:
  kvector(){
    elems=NULL;
    max_elem=0;
    num_elem=0;
  }
  kvector(size_t max_elem){
    //    kprintf("constractor()\n");
    this->max_elem = max_elem;
    this->num_elem = max_elem;
    elems = new TYPE[max_elem];
  }
  kvector(const kvector<TYPE> &copied){
    //    kprintf("copy constractor()\n");
    copy(copied);
  }
  ~kvector(){
    //    kprintf("destractor()\n");
    if(elems!=NULL)
      delete[] elems;
    //    kprintf("destractor() end\n");
  }
  int length(){
    return num_elem;
    //    return max_elem;
  }
  TYPE &operator [](size_t num_elem)const{
    if(num_elem >= max_elem)
      return *(TYPE *)NULL;
    return elems[num_elem];
  }
  bool set_nr_elem(size_t nr_elem){
    if(nr_elem < max_elem){
      num_elem = nr_elem;
      return true;
    }else{
      return false;
    }
  }
  kvector<TYPE>& operator=(const kvector<TYPE> &copied){
    if(elems!=NULL){
      delete[] elems;
    }
    copy(copied);
    return *this;
  }
};


// class virString{
// protected:
//   int length;
//   char *pstring;
// public:
//   const char *toChar()const{
//     return pstring;
//   }
//   int size()const{
//     return length;
//   }
// };

// //template <int N>
// class kstring : punnblic virString{
//   char *string;
//  public:
//   kstring(){
//     pstring=string;
//     length=0;
//   }
//   kstring(const char *c){
//     pstring=string;
//     length=strlen(c)+1;
//     if(length<N){
//       strncpy(string,c,length);
//     }else{
//       length=0;
//     }
//   }
//   kstring(const virString toCopy){
//     pstring=string;
//     if(toCopy.size()<length){
//       length=toCopy.size();
//       strncpy(string,toCopy.toChar(),length);
//     }else{
//       length=0;
//     }
//   }
//   /*
//   const String &operator=(const virString rh){
    
//   }
//   */
//   const kstring &operator=(const char *rh){
//     int size=strlen(rh)+1;
//     if(size<length){
//       strncpy(string,rh,size);
//     }
//   }
// };

#endif
