

/*H**********************************************************************
* FILENAME :        server.c
*
* DESCRIPTION :
*       This is a program which should be able to send messenges to a
*       client which should also be able to handle the messenges.
*       This is therefor a part of a bigger apllicaation layer.
*
* NOTES :
*       This is a part of a project done in school.
*
* AUTHOR :    Mikkel Schmidt Andersen        START DATE :    13 Sep 19
*
* CHANGES : Nothing yet-
*
* REF NO          VERSION     DATE
* Serverhandler   V.0.0.1    13Sep19
*
*************************************************************************/

#define PORT "80"

#define PENDINGCONNECTIONS 10 //This variable should tell how many connections our queue can hold.

//This fuction is supposed to get our sockaddr aka our IPv4.
//Soo lets hope it works ! ;)
void *get_in_addr(struct sockaadr *sa)
{
  if(sa->sa_family == AF_INET){
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }
  return &(((struct sockaadr_in6*)sa)->sin6_addr);
}


int main(void)
{





  return 0;
}
