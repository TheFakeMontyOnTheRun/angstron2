//using namespace std;
#include "BZK_POSIX.h"
#include "BZK_BaseIncludes.h"
#include "Vacuum/VAC_BaseIncludes.h"

VAC_Applet *GameApplet;
int pipe1[2], pipe0[2];
int pid = 0;
#define MAXDATASIZE 100 // max number of bytes we can get at once 
/*
void StartPipeMultiplayerSession()
{




    pipe(pipe1);
    pipe(pipe0);

    pid = fork();


    if (pid) {
        char buffer[7];
        close(pipe1[0]);
        close(pipe0[1]);
        write(pipe1[1], "player1", 7 * sizeof (char));
        sleep(2);
        read(pipe0[0], &buffer, 7 * sizeof (char));
        //    cout << "recebi notícias de " << buffer << endl;

    } else {
        char buffer[7];
        close(pipe1[1]);
        close(pipe0[0]);
        write(pipe0[1], "player2", 7 * sizeof (char));
        sleep(2);
        read(pipe1[0], &buffer, 7 * sizeof (char));

        //  cout << "recebi notícias de " << buffer << endl;
    }


}
*/

#ifdef NETWORKSUPPORT
bool ConnectToServer(string hostname)
{
    char buf[MAXDATASIZE];
    int numbytes = 0;
    string msg;
    int new_fd; // listen on sock_fd, new connection on new_fd

    int sockfd = init_client((char*) hostname.c_str(), "24243");
    if (sockfd==-1) return false;
    cout << "init client ok" << endl;
    int myserver = 0;
    int answer = 0;



    if (myserver == 0) {
        myserver = init_server("24244");
        cout << "server initialized @" << myserver << endl;
    }
    if (answer == 0) {
        answer = waitforclient(myserver, msg);
        cout << "waiting for devices @" << answer << endl;
    }
    numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0);
    buf[numbytes] = '\0';
    msg = "connect";
    send(answer, msg.c_str(), msg.length(), 0);
    pipe1[1] = answer;
    pipe0[0] = sockfd;
    pid = 1;
    return true;
}

void StartServer()
{
    char buf[MAXDATASIZE];
    int numbytes = 0;
    int sockfd;
    int new_fd; // listen on sock_fd, new connection on new_fd
    int myclient = -1024;
    string msg;
    string clientip;
    sockfd = init_server("24243");

    printf("server: waiting for connections...\n");
    new_fd = waitforclient(sockfd, clientip);
    cout << "recebendo conexão de " << clientip << endl;
    msg = "connect";
    sleep(5);
    send(new_fd, msg.c_str(), msg.length(), 0);
    if (myclient == -1024) {
        myclient = init_client((char*) clientip.c_str(), "24244");
        cout << "connecting back @" << myclient << endl;
    }
    numbytes = recv(myclient, buf, MAXDATASIZE - 1, 0);
    buf[numbytes] = '\0';
    msg = buf;
    cout << "veio:" << msg << " em " << numbytes << " bytes" << endl;
    pipe0[1] = new_fd;
    pipe1[0] = myclient;
    pid = 0;
}
#endif
/*
string GetMyIp()
{
string toreturn;
int device=0;

struct ifaddrs * ifAddrStruct=NULL;
void * tmpAddrPtr=NULL;
int i=0;
char addressBuffer[INET_ADDRSTRLEN];
getifaddrs(&ifAddrStruct);

while (ifAddrStruct!=NULL) {
  if (ifAddrStruct->ifa_addr->sa_family==AF_INET && strcmp(ifAddrStruct->ifa_name, "lo0")!=0) { // check it is IP4 and not lo0
    // is a valid IP4 Address
    device++;    
    tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
    toreturn=inet_ntop(AF_INET, tmpAddrPtr, addressBuffer,INET_ADDRSTRLEN);
    if (device==2) break;
  }
  ifAddrStruct=ifAddrStruct->ifa_next;
}
return toreturn;
}


void SearchServer()
{
	string myip=GetMyIp();
	string subnet;
	string myid;
	string currentid;
	string currentip;
	string tmp=myip;
	bool gotit=false;
	cout << "my ip:"<<myip <<endl;

	subnet+=BZK_ParsingTools::GetStringUntilSep(tmp,".");
	subnet+=".";	
	tmp=BZK_ParsingTools::GetStringFromSep(tmp,".");

	subnet+=BZK_ParsingTools::GetStringUntilSep(tmp,".");
	subnet+=".";	
	tmp=BZK_ParsingTools::GetStringFromSep(tmp,".");

	subnet+=BZK_ParsingTools::GetStringUntilSep(tmp,".");
	subnet+=".";	
	tmp=BZK_ParsingTools::GetStringFromSep(tmp,".");
	myid=BZK_ParsingTools::GetStringUntilSep(tmp,".");
	for (int c=1;c<255;c++)
		{
		currentid=BZK_FastMath::Print(BZK_FastMath::Int32toFix32(c));
		//if (currentid==myid) continue;
		currentip=subnet+currentid;
		cout << "tentando com "<<currentip<<endl;
		gotit=ConnectToServer((char*)currentip.c_str());
		if (gotit) return;
		}

	cout << subnet <<endl;


    return;
    }*/

extern  void JoinGame()
{
#ifdef NETWORKSUPPORT
		string tmp;  
    		tmp=WaitForBroadcast();
		sleep(2);
    		ConnectToServer(tmp);
#endif
}

extern void StartNetworkedGame()
{
#ifdef NETWORKSUPPORT
		GlobalBroadcast();
    		StartServer();
#endif
}


extern void NET_WaitForPlayer()
{
#ifdef NETWORKSUPPORT
                if (pid) {
	            int tmp;
                    read(pipe0[0], &tmp, 4);
                } else {
                    int tmp;
                    read(pipe1[0], &tmp, 4);
                }
#endif
}


extern void NET_SignalPlayer()
{
#ifdef NETWORKSUPPORT
                if (pid) {
	            int tmp=1;
                    write(pipe1[1], &tmp, 4);
                } else {
                    int tmp=1;
                    write(pipe0[1], &tmp, 4);
                }
#endif
}


 void BZKmain(int argc, char *argv[])
{

int Time[2];
/*
Time[0]=time(0);
for (int c=0;c<0x8FFFFFF;c++)
	BZK_FastMath::Fix32toReal32(BZK_FastMath::Real32toFix32(3.14159265));
Time[1]=time(0);
std::cout << Time[1]-Time[0]<< "-Pow2-"<<BZK_FastMath::Fix32toReal32(BZK_FastMath::Real32toFix32(3.14159265)) << std::endl;

Time[0]=time(0);
for (int c=0;c<0x8FFFFFF;c++)
	BZK_FastMath::_Fix32toReal32(BZK_FastMath::_Real32toFix32(3.14159265));
Time[1]=time(0);
*/
//std::cout << Time[1]-Time[0]<< ";mul:"<<BZK_FastMath::Fix32toInt32(BZK_FastMath::Mul(BZK_FastMath::Int32toFix32(31415),BZK_FastMath::Int32toFix32(2))) << std::endl;


//exit(0);

//for (int c=-6;c<8;c++)		cout<<"c="<<c<<"|->"<<BZK_FastMath::GetOpositeDirection(c)<<endl;   return;  
int num;
  int tmp;
   BZK_FixP tmp2;	
VAC_Actor *actor;
    cout << "using " << STOREPATH << " as storage area" << endl;
    cout << "using math for " << sizeof(BZK_FixP) << " bytes (FPU/FIXP)" << endl;
    GameApplet = NULL;
    //   std::cout << sizeof(BZK_FixP) <<endl;    
    //   StartPipeMultiplayerSession();
    // connected=true;
    
    
    GameApplet = new VAC_Applet();
    atexit(BZK_Quit);
    if (argc > 0)
      GameApplet->SetApplicationName(argv[0]);
        else
     GameApplet->SetApplicationName("hunter");


    if (GameApplet == NULL)
        exit(ERR_OUT_OF_MEMORY);

    BZK_SystemEvent SystemEvents;
    bool quit = 0;


    GameApplet->Start();

    while (!quit) {
#ifdef NETWORKSUPPORT
        if ( GameApplet->IsConnected() && GameApplet->GetDefaultGameView()==GameApplet->GetCurrentView()) {
	    if  (GameApplet->GetDefaultGameView()->GetActors()->GetActorsCount() <= 1)
		NET_InitOK=0;


            if (!NET_InitOK && GameApplet->GetDefaultGameView()->GetActors()->GetActorsCount() > 1) {
                if (pid) {
                    ((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(0))->SetClass(11);
                    ((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(2))->SetClass(4096);

                } else
                	{
               		((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(2))->SetClass(11);
	                ((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(0))->SetClass(4096);
                	}	

                NET_InitOK = 1;
            }

            if (GameApplet->GetDefaultGameView()->GetActors()->GetActorsCount() > 1) {
                if (pid)
                    GameApplet->GetDefaultGameView()->GetActors()->SetActivePlayer(0);
                else
                    GameApplet->GetDefaultGameView()->GetActors()->SetActivePlayer(2);
                    
                if (pid) {

                    tmp =GameApplet->GetDefaultGameView()->lastkeycode;
                    write(pipe1[1], &tmp, 4);

                    read(pipe0[0], &num, 4);
		  /*
		    for (int c=num;c<GameApplet->GetDefaultGameView()->GetActors()->GetActorsCount();c++)
			{
			actor=new VAC_Actor();
			GameApplet->GetDefaultGameView()->GetActors()->AddActor(*actor);
			}
		*/
			if (num > GameApplet->GetDefaultGameView()->GetActors()->GetActorsCount())
				num=GameApplet->GetDefaultGameView()->GetActors()->GetActorsCount();
		    for (int c=0;c<num;c++)
			{

			if (c==0 && c==2) continue;

	                    read(pipe0[0], &tmp, 4);
		            ((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(c))->SetAngle(tmp);
	                    read(pipe0[0], &tmp2, 4);
		            ((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(c))->GetPosition()->SetX(tmp2);
	                    read(pipe0[0], &tmp2, 4);
		            ((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(c))->GetPosition()->SetY(tmp2);
	                    read(pipe0[0], &tmp2, 4);
		            ((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(c))->GetPosition()->SetZ(tmp2);
	                    read(pipe0[0], &tmp, 4);
		            ((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(c))->SetCurrentSector(tmp);
			}

                    read(pipe0[0], &tmp, 4);
                    ((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(2))->PushEvent(tmp,1);
		    GameApplet->GetDefaultGameView()->GetWorld()->peersector=GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(2)->GetCurrentSector();

                } else {
                    int tmp;


		    read(pipe1[0], &tmp, 4);
                    ((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(0))->PushEvent(tmp,1);               
		    GameApplet->GetDefaultGameView()->GetWorld()->peersector=GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(0)->GetCurrentSector();
		   num=GameApplet->GetDefaultGameView()->GetActors()->GetActorsCount();
		    write(pipe0[1], &num, 4);
		    for (int c=0;c<num;c++)
			{
			if (c==0 && c==2) continue;

		            tmp=((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(c))->GetAngle();
	                     write(pipe0[1], &tmp, 4);
		            tmp2=((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(c))->GetPosition()->GetX();
	                     write(pipe0[1], &tmp2, 4);
		            tmp2=((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(c))->GetPosition()->GetY();
	                     write(pipe0[1], &tmp2, 4);
		            tmp2=((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(c))->GetPosition()->GetZ();
			     write(pipe0[1], &tmp2, 4);
		            tmp=((VAC_Actor*) GameApplet->GetDefaultGameView()->GetActors()->GetActorPtr(c))->GetCurrentSector();
			     write(pipe0[1], &tmp, 4);
			}
                    tmp =GameApplet->GetDefaultGameView()->lastkeycode;       
                    write(pipe0[1], &tmp, 4);
                    
                }

            }
		else std::cout << "**less than two**" << std::endl;
        }
#endif


        GameApplet->Refresh();
        SDL_PollEvent(&SystemEvents);
        GameApplet->FilterEvents(&SystemEvents);
        quit = GameApplet->WaitForNextCycle();
    }

    delete GameApplet;
}
