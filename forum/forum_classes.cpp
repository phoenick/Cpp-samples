#include <iostream>
#include <string>
#include <list>
#include <stdlib.h>
#include <unistd.h> //for unix :unistd.h and sleep(sec)
#include "forum_classes.h"

using namespace std;

forum_system::forum_system(string title){
   LastForumID = 0;
   LastThreadID = 0;
   LastPostID = 0;
   LastUserID = 0;
   system_title = title;

}

void forum_system::showSystemInterface(bool mode){
 string username, password;
 if(mode == normal){
// TODO (Hastoukas#1#): -Log in environment\
-take from standard input username and password\
-find the user whose  personal information corresponds\
 with these\
-fix the logged_in_user pointer in order to point to him
   cout << "Welcome to " + system_title << endl;
   cout << "Please type your username and password" << endl;
   cout << "username:" << flush;
   getline(cin, username);
   //todo: check username for inappropriate characters(spaces e.t.c)
   cout << "password:" << flush;
   getline(cin, password);
   //todo: check password for inappropriate characters(spaces e.t.c)

   if(username != ""){
      list<user> ::iterator i;
      for(i = user_list.begin(); i != user_list.end(); ++i)
         if((*i).get_username() == username){
            logged_in_user = &(*i);
            break;
         }
      if((i == user_list.end()) || password != (*i).get_password()){
         cout << "Invalid username or password !\nThe program will be terminated!" << endl;
         return;
      }
   }
   else{
      logged_in_user = new user;
   }
}
else{
   cout << "->> Welcome to " + system_title + " <<-" << endl;
   cout << "* User Registration Mode *" << endl;
   cout << "Please type the desired username and password "<< endl;
   cout << "username:" << flush;
   getline(cin, username);
   cout << "password:" << flush;
   getline(cin, password);
   if(username != "" && password != ""){
      list<user> ::iterator i;
      for(i = user_list.begin(); i != user_list.end(); ++i)
         if((*i).get_username() == username){
            cout << "Given username is not available!\nThe program will be terminated!" << endl;
            return;
         }
      logged_in_user = new user(username, password, LastUserID + 1);
      LastUserID++;
      user_list.push_back(*logged_in_user);
   }
   else{
      cout << "Registration failed due to invalid given username or password!" << endl;
      cout << "The program will be terminated!" << endl;
      return;
   }
}
system("clear");
//========================================================//
list<forum> ::iterator fli;
NavigationInformation nInfo;
nInfo = showStartScreen();
while(true){
   switch(nInfo.navi){
      case visitForum: nInfo = showForumVScreen(*nInfo.vfi);
                               break;
      case returntoSS: nInfo = showStartScreen();
                                break;
      case exitProgramSCS: saveCurrentState();
                                        return;
      case exitProgram: return;
   }
}
}
//////////////////////////////////////////////////////////
NavigationInformation forum_system::showStartScreen(){
  NavigationInformation nInfo;
  UsefulFunctions usefunc;
 while(true){
    cout << "Welcome " +  logged_in_user->get_username() +"!\n" << endl;
    int linuser_group = logged_in_user->get_user_group();
    cout << "You are logged in as ";
    switch(linuser_group){
       case guest: cout << "\"guest\"\n" << endl;
                          break;
       case registered_user: cout << "\"registered user\"\n" << endl;
                                        break;
       case global_moderator: cout << "\"global moderator\"\n" << endl;
                                          break;
       case administrator: cout << "\"administrator\"\n" << endl;
                                    break;
    }

   cout << "Available forums : " << endl;
   list<forum> ::iterator fi;
   for(fi = forum_list.begin(); fi != forum_list.end() ; fi++){
      cout << fi->get_forum_title() << " (ID: " << fi->get_forum_ID() << ")" << endl;
   }
   cout << endl;
   //TODO : show forums and threads that exist\
at current level


   cout << "Type : " << endl;
   cout << "the ID of the forum you want to visit" << endl;
   cout << "X to exit without saving current state" << endl;
   if(linuser_group > guest){
      cout << "L to save current state and exit" << endl;
      if(linuser_group > global_moderator){
         cout << "N to create a new forum" << endl;
         cout << "U to manage the users" << endl;
      }
   }

   string cmd;
   getline(cin, cmd);
   if(cmd.size() > 3 || cmd.size() == 0){
      cout << "Invalid command ! Please type a valid command." << endl;
      sleep(2);
      system("clear");
      continue;
   }

   string :: const_iterator i;
   i = cmd.begin();
   char c = *i;

   switch(c){
      case 'X': nInfo.navi = exitProgram;
                     system("clear");
                     return nInfo;
                    //break;
      case 'L': if(linuser_group > guest){
                     nInfo.navi = exitProgramSCS;
                     system("clear");
                     return nInfo;
                    }
                    else{
                       cout << "Invalid command ! Please type a valid command." << endl;
                       sleep(2);
                       system("clear");
                    }
                    break;
      case 'N': if(linuser_group == administrator){
                        string nftitle;
                        list<forum>:: iterator fli;
                        forum *fptr;
                        fptr = new forum;
                        forum_list.push_back(*fptr);
                        delete fptr;
                        fli = forum_list.end();
                        fli--;
                        cout << "Please enter the title of the new forum" << endl;
                        getline(cin, nftitle);
                        fptr = new forum(nftitle, ++LastForumID, fli);
                        (*fli) = *fptr;
                        delete fptr;
                        system("clear");
                     }
                     else{
                       cout << "Invalid command ! Please type a valid command." << endl;
                       sleep(2);
                       system("clear");
                    }
                    break;
       case 'U': if(linuser_group == administrator){
                        system("clear");
                        string cmd;
                        string :: const_iterator i;
                        char c;
                        bool exitUMM = false;
                        while(!exitUMM){
                           cout << "C to see a list with all the users" << endl;
                           cout << "M to change the group where a user belongs" << endl;
                           cout << "D to delete a user" << endl;
                           cout << "U to change the name of a user" << endl;
                           cout << "P to change the password of a user" << endl;
                           cout << "R to return to the start screen" << endl;
                           cout << "L to save current state and exit" << endl;
                           cout << "X to exit without saving current state" << endl;

                           getline(cin, cmd);
                           i = cmd.begin();
                           c =*i;
                           if(cmd.size() >1 || c < 65 || c > 90){
                              cout << "Invalid command ! Please type a valid command." << endl;
                              sleep(2);
                              system("clear");
                              continue;
                           }
                           switch(c){
                              case 'C': system("clear");
                                            displayUsersList();
                                            break;
                              case 'M':{
                                            cout << "Please enter the ID of the user who you want to change his group" << endl;
                                            string lcmd;
                                            getline(cin, lcmd);
                                            int enteredID = usefunc.string2int(lcmd);
                                            if(enteredID == -1){
                                                cout << "Invalid command ! Please type a valid command." << endl;
                                                sleep(2);
                                                system("clear");
                                                break;
                                            }
                                            list<user> ::iterator uli;
                                            if( findUser(user_list, enteredID, uli) ){
                                                int engroup;
                                                cout << "Please enter the new group for the indicated user (1, 2 or 3)" << endl;
                                                getline(cin, lcmd);
                                                engroup = usefunc.string2int(lcmd);
                                                if(engroup == -1){
                                                   cout << "Invalid command ! Please type a valid command." << endl;
                                                   sleep(2);
                                                   system("clear");
                                                   break;
                                                }
                                                else if(engroup < 1 || engroup > 3){
                                                   cout << "Error: Entered group code is invalid!" << endl;
                                                   sleep(2);
                                                   system("clear");
                                                   break;
                                                }
                                                else{
                                                   uli->set_user_group(engroup);
                                                   system("clear");
                                                   break;
                                                }
                                            }
                                            else{
                                               cout << "Error: Indicated user was not found! (entered ID does not exist)" << endl;
                                               sleep(2);
                                               system("clear");
                                               break;
                                            }
                                          }
                              case 'D': {
                                            cout << "Please enter the ID of the user who you want to change his name" << endl;
                                            string lcmd;
                                            getline(cin, lcmd);
                                            int enteredID = usefunc.string2int(lcmd);
                                            if(enteredID == -1){
                                                cout << "Invalid command ! Please type a valid command." << endl;
                                                sleep(2);
                                                system("clear");
                                                break;
                                            }
                                            list<user> ::iterator uli;
                                            if( findUser(user_list, enteredID, uli) ){
                                                user_list.erase(uli);
                                            }
                                            else{
                                               cout << "Error: Indicated user was not found! (entered ID does not exist)" << endl;
                                               sleep(2);
                                               system("clear");
                                               break;
                                            }
                                          }
                              case 'U':{
                                            cout << "Please enter the ID of the user who you want to change his name" << endl;
                                            string lcmd;
                                            getline(cin, lcmd);
                                            int enteredID = usefunc.string2int(lcmd);
                                            if(enteredID == -1){
                                                cout << "Invalid command ! Please type a valid command." << endl;
                                                sleep(2);
                                                system("clear");
                                                break;
                                            }
                                            list<user> ::iterator uli;
                                            if( findUser(user_list, enteredID, uli) ){
                                                string enusername;
                                                cout << "Please enter the new name for the indicated user " << endl;
                                                getline(cin, enusername);

                                                if(!usefunc.chkString(enusername)){
                                                   cout << "Error: Entered username is invalid" << endl;
                                                   sleep(2);
                                                   system("clear");
                                                   break;
                                                }
                                                else{
                                                   uli->set_username(enusername);
                                                   system("clear");
                                                   break;
                                                }
                                            }
                                            else{
                                               cout << "Error: Indicated user was not found! (entered ID does not exist)" << endl;
                                               sleep(2);
                                               system("clear");
                                               break;
                                            }
                                          }
                              case 'P':{
                                            cout << "Please enter the ID of the user who you want to change his name" << endl;
                                            string lcmd;
                                            getline(cin, lcmd);
                                            int enteredID = usefunc.string2int(lcmd);
                                            if(enteredID == -1){
                                                cout << "Invalid command ! Please type a valid command." << endl;
                                                sleep(2);
                                                system("clear");
                                                break;
                                            }
                                            list<user> ::iterator uli;
                                            if( findUser(user_list, enteredID, uli) ){
                                                string enpassword;
                                                cout << "Please enter the new name for the indicated user " << endl;
                                                getline(cin, enpassword);

                                                if(!usefunc.chkString(enpassword)){
                                                   cout << "Error: Entered password is invalid" << endl;
                                                   sleep(2);
                                                   system("clear");
                                                   break;
                                                }
                                                else{
                                                   uli->set_password(enpassword);
                                                   system("clear");
                                                   break;
                                                }
                                            }
                                            else{
                                               cout << "Error: Indicated user was not found! (entered ID does not exist)" << endl;
                                               sleep(2);
                                               system("clear");
                                               break;
                                            }
                                          }
                              case 'R': exitUMM = true;
                                            system("clear");
                                            break;
                              case 'L':  nInfo.navi = exitProgramSCS;
                                             return nInfo;
                              case 'X': nInfo.navi = exitProgram;
                                             return nInfo;
                              default: cout << "Invalid command ! Please type a valid command." << endl;
                                           sleep(2);
                                           system("clear");
                                           break;
                           }
                        }
                     }
                     else{
                       cout << "Invalid command ! Please type a valid command." << endl;
                       sleep(2);
                       system("clear");
                    }
                    break;
       default: {
                        int enteredID = usefunc.string2int(cmd);
                        if(enteredID == -1){
                           cout << "Invalid command ! Please type a valid command." << endl;
                           sleep(2);
                           system("clear");
                           break;
                        }
                        //if everything ok so far with the string entered:
                        list<forum> ::iterator lfi;
                        for(lfi = forum_list.begin(); lfi != forum_list.end(); lfi++){
                           if(lfi->get_forum_ID() == enteredID){
                              system("clear");
                              nInfo.navi = visitForum;
                              nInfo.vfi = lfi;
                              return nInfo;
                           }
                        }
                        cout << "The ID entered does not correspond to an existing forum ! " << endl;
                        sleep(2);
                        system("clear");
                        break;

                     }

   }
 }
}

NavigationInformation forum_system::showForumVScreen(forum& vForum){

   NavigationInformation nInfo;
   UsefulFunctions usefunc;
   int linuser_group = logged_in_user->get_user_group();
   list<forum> ::iterator vfli;

   while(true){
      system("clear");
      cout << "You are in the forum : " + vForum.get_forum_title() << end << endl;
      cout << "Available forums : " << endl;
      for(vfli = vForum.get_fli(beginning); vfli != vForum.get_fli(end); vfli++){
         cout << vfli->get_forum_title() << " (ID: " << vfli->get_forum_ID() << ")" << endl;
      }
      cout << end << endl;

      cout << "Available threads : " << endl;
      vForum.showAvThreads();
      cout << end << endl;
      cout << "Type:" << endl;
      cout << "F to visit a forum from the above" << endl;
      cout << "T to visit a thread from the above" << endl;
      cout << "B to go one level up the hierarchy" << endl;
      cout << "H to go to the start screen" << endl;
      if(linuser_group > guest){
         cout << "C to create a new thread" << endl;
         if(linuser_group == administrator){
            cout << "N to create a new forum" << endl;
            cout << "E to change this forum's title" << endl;
            cout << "D to delete this forum" << endl;
            cout << "M to move this forum" << endl;
         }
         cout << "L to save current state and exit" << endl;
      }
      cout << "X to exit without saving current state" << endl; //TODO: make the SS menu like this

      string cmd;
      getline(cin, cmd);
      if(cmd.size() > 1 || cmd.size() == 0){
         cout << "Invalid command ! Please type a valid command." << endl;
         sleep(2);
         system("clear");
         continue;
      }

      string :: const_iterator i;
      i = cmd.begin();
      char c = *i;
      switch(c){
         case 'F':  {
                        cout << "Please type the ID of the forum you want to visit" << endl;
                        getline(cin, cmd);
                        if(cmd.size() > 3 || cmd.size() == 0){
                           cout << "Invalid command ! Please type a valid command." << endl;
                           sleep(2);
                           system("clear");
                           break;
                        }

                        int enteredID = usefunc.string2int(cmd);
                        if(enteredID == -1){
                           cout << "Invalid command ! Please type a valid command." << endl;
                           sleep(2);
                           system("clear");
                           break;
                        }
                        //if the entered string is indeed a number then check if any forum
                        //corresponds to that
                           for(vfli = vForum.get_fli(beginning); vfli != vForum.get_fli(end); vfli++){
                              if(vfli->get_forum_ID() == enteredID){
                              nInfo.navi = visitForum;
                              nInfo.vfi = vfli;
                              system("clear");
                              return nInfo;
                              }
                           }
                        //if no forum found that owns the entered ID:
                        cout << "The ID entered does not correspond to an existing forum ! " << endl;
                        sleep(2);
                        system("clear");
                        break;
                        }
         case 'T': {
                        cout << "Please type the ID of the thread you want to visit" << endl;
                        getline(cin, cmd);
                        if(cmd.size() > 3 || cmd.size() == 0){
                           cout << "Invalid command ! Please type a valid command." << endl;
                           sleep(2);
                           system("clear");
                           break;
                        }

                        int enteredID = usefunc.string2int(cmd);
                        if(enteredID == -1){
                           cout << "Invalid command ! Please type a valid command." << endl;
                           sleep(2);
                           system("clear");
                           break;
                        }
                        //if nothing is wrong so far with the string entered
                        //let the forum handle the situation henceforth as it thinks better
                        nInfo = vForum.visitThread(enteredID, linuser_group, LastPostID, logged_in_user->get_username());
                        if(nInfo.navi != nop)
                           return nInfo;
                        else
                           break;
                        }

         case 'B': if(vForum.isUnderSystem()){
                           nInfo.navi = returntoSS;
                           system("clear");
                           return nInfo;
                        }
                        else{
                           nInfo.navi = visitForum;
                           nInfo.vfi = vForum.get_parenti();
                           system("clear");
                           return nInfo;
                        }
         case 'H':nInfo.navi = returntoSS;
                       system("clear");
                       return nInfo;
         case 'C':if(linuser_group > guest){
                        string ntTitle, fpContent;
                        cout << "Please enter the title of the new thread" << endl;
                        getline(cin, ntTitle);
                        cout << "Please enter the content of the first post" << endl;
                        getline(cin, fpContent);
                        vForum.addThread(ntTitle, ++LastThreadID, false, logged_in_user->get_username(), fpContent, ++LastPostID);
                        system("clear");
                        break;
                      }
                      else{
                        cout << "Invalid command ! Please type a valid command." << endl;
                        sleep(2);
                        system("clear");
                        break;
                      }
         case 'N':if(linuser_group == administrator){
                           string nftitle;
                           cout << "Please enter the title of the new forum" << endl;
                           getline(cin, nftitle);
                           vForum.createSubforum(nftitle, ++LastForumID);
                           system("clear");
                           break;
                       }
                       else{
                        cout << "Invalid command ! Please type a valid command." << endl;
                        sleep(2);
                        system("clear");
                        break;
                      }
         case 'E':if(linuser_group == administrator){
                           string nftitle;
                           cout << "Please enter the new title of the forum" << endl;
                           getline(cin, nftitle);
                           vForum.set_forum_title(nftitle);
                           system("clear");
                           break;
                      }
                      else{
                        cout << "Invalid command ! Please type a valid command." << endl;
                        sleep(2);
                        system("clear");
                        break;
                      }
         case 'D':if(linuser_group == administrator){
                           list<forum>::iterator fli;
                           if(vForum.isUnderSystem()){
                              nInfo.navi = returntoSS;
                              forum_list.erase(vForum.get_selfi());
                              system("clear");
                              return nInfo;
                           }
                           else{
                              fli = vForum.get_parenti();
                              nInfo.navi = visitForum;
                              nInfo.vfi = fli;
                              fli->deleteForum(vForum.get_selfi());
                              system("clear");
                              return nInfo;
                           }
                        }
                        else{
                        cout << "Invalid command ! Please type a valid command." << endl;
                        sleep(2);
                        system("clear");
                        break;
                      }
         case 'M':break;
         case 'L': nInfo.navi = exitProgramSCS;
                       system("clear");
                       return nInfo;
         case 'X': nInfo.navi = exitProgram;
                       system("clear");
                       return nInfo;

         default:cout << "Invalid command ! Please type a valid command." << endl;
                     sleep(2);
                     system("clear");
                     break;
      }
   }
}

void forum_system::displayUsersList(){
   list<user> ::iterator uli;
   cout << "Users' List with their  ";
   for(uli = user_list.begin(); uli != user_list.end(); uli++){
      cout << uli->get_username()<< " " << uli->get_user_group()<< " "<<uli->get_user_ID() << endl;
   }

}

bool forum_system::findUser(const list<user>& gusr_list, int usrID, list<user> ::iterator& uli){
   list<user> ::iterator uf;
   for(uf = user_list.begin(); uf != user_list.end(); uf++)
      if( usrID == uf->get_user_ID() ){
         uli = uf;
         return true;
      }

   return false;
}

void forum_system::saveCurrentState(){}

/*---------------------------------------------*/
user::user(){
   user_ID = 0;
   user_group = guest;
   username = "dear guest";
}
user::user(string personal_information){}
user::user(string gusername, string gpassword, int guser_ID){
   user_group = administrator;//<-------posibility to change----
   user_ID = guser_ID;
   username = gusername;
   password = gpassword;
}

 string user::get_username() const{
    return username;
 }
string user::get_password() const{
   return password;
}
int user::get_user_group() const{
   return user_group;
}
int user::get_user_ID() const{
   return user_ID;
}

void user::set_username(const string& nuname){
   username = nuname;
}
void user::set_password(const string& npasswd){
   password = npasswd;
}
void user::set_user_group(const int nugroup){
   if(nugroup < 1 || nugroup > 3){
      cout << "Fatal error!" << endl;
      exit(1);
   }
   else
      user_group = nugroup;
}
void user::set_user_ID(const int nuID){
   user_ID = nuID;
}
///////////////////////////////////////
int post::get_post_ID(){
   return post_ID;
}
string post::get_post_content(){
   return content;
}
string post::get_sender_username(){
   return sender_username;
}

post::post(const string& pcont, int pID, const string& susrname){
   content = pcont;
   post_ID = pID;
   sender_username = susrname;
}
///////////////////////////////////////
int thread::get_thread_ID(){
   return thread_ID;
}

string thread::get_thread_title(){
   return thread_title;
}

string thread::get_creator_username(){
   return creator_username;
}

bool thread::isLocked(){
   return locked;
}

void thread::addPost(const string& pcont, int pID, const string& susrname){
      post fp(pcont, pID, susrname);
      post_list.push_back(fp);
}

void thread::deletePost(int pid){
   list<post>::iterator pli;
   for(pli = post_list.begin(); pli != post_list.end(); pli++){
      if(pid == pli->get_post_ID()){
         post_list.erase(pli);
         return;
      }
   }
   cout << "Entered ID does not correspond to an existing post! Deletion canceled!" << endl;
}

void thread::showPosts(){
   list<post> ::iterator pli;
   for(pli = post_list.begin(); pli != post_list.end(); pli++){
      cout << pli->get_sender_username() + ": " + pli->get_post_content() + "(ID: "<< pli->get_post_ID() << ")" << endl;
   }
   cout << endl;
}

void thread::set_thread_title(const string& nttitle){
   thread_title = nttitle;
}

void thread::toggleLock(){
   if(locked)
      locked = false;
   else
      locked = true;
}

thread::thread(const string& title, int tID, const string& cusrname, bool lcd){
   thread_title = title;
   thread_ID = tID;
   creator_username = cusrname;
   locked = lcd;
}
///////////////////////////////////////
int forum::get_forum_ID() const{
   return forum_ID;
}
string forum::get_forum_title() const{
   return forum_title;
}
list<forum> ::iterator forum::get_selfi(){
   return selfi;
}
list<forum> ::iterator forum::get_parenti(){
   return parenti;
}
NavigationInformation forum::visitThread(int threadID, int linuser_group, int& LastPostID, const string& linusrname){
   NavigationInformation nInfo;
   UsefulFunctions usefunc;
   list<thread> ::iterator tli;
   bool tSticky = false;
   bool indtf = false; //indtf :indicated thread found
   for(tli = sticky_thread_list.begin(); tli != sticky_thread_list.end(); tli++){
      if(tli->get_thread_ID() == threadID){
         indtf = true;
         tSticky = true;
         break;
      }
   }
   if(indtf != true){
      for(tli = thread_list.begin(); tli != thread_list.end(); tli++){
         if(tli->get_thread_ID() == threadID){
            indtf = true;
            break;
         }
      }
   }
   if(indtf == true){
      system("clear");
      while(true){
         cout << "You are in the forum :" + forum_title << endl;
         cout << "You are in the thread :" + tli->get_thread_title() << end << endl;
         cout << "Posts :" << endl;
         tli->showPosts();
         cout << "Type:" << endl;
         cout << "B to go one level up the hierarchy" << endl;
         cout << "H to go to the start screen" << endl;
         if(linuser_group > guest){
            cout << "R to answer on the thread" << endl;
            if(linuser_group > registered_user){
               cout << "D to delete this thread" << endl;
               cout << "M to move this thread somewhere else" << endl;
               cout << "E to change the title of this thread" << endl;
               cout << "Y to toggle this thread's \"stickyness\"" << endl;
               cout << "K to lock or to unlock this thread" << endl;
               cout << "A to delete a post" << endl;
               cout << "S to move a post" << endl;
            }
            cout << "L to save current state and exit" << endl;
         }
         cout << "X to exit without saving current state" << endl;

         string cmd;
         getline(cin, cmd);
         string :: const_iterator i;
         i = cmd.begin();
         char c = *i;
         if(cmd.size() > 1 || c < 65 || c > 90){
            cout << "Invalid command ! Please type a valid command." << endl;
            sleep(2);
            system("clear");
            continue;
         }
         switch(c){
            case 'B': nInfo.navi = nop;
                          system("clear");
                          return nInfo;
            case 'R': if(linuser_group > guest){
                            if(!tli->isLocked()){
                              string npc;
                              cout << "Please enter your post" << endl;
                              getline(cin, npc);
                              tli->addPost(npc, ++LastPostID, linusrname);
                              system("clear");
                              break;
                            }
                            else{
                              cout << "Sorry ! This thread is locked!" << endl;
                              sleep(2);
                              system("clear");
                              break;
                            }
                           }
                           else{
                              cout << "Invalid command ! Please type a valid command." << endl;
                              sleep(2);
                              system("clear");
                              break;
                           }

            case 'H': nInfo.navi = returntoSS;
                           system("clear");
                           return nInfo;
            case 'D': if(linuser_group > registered_user){
                              thread_list.erase(tli);
                              nInfo.navi = nop;
                              system("clear");
                              return nInfo;
                           }
                           else{
                              cout << "Invalid command ! Please type a valid command." << endl;
                              sleep(2);
                              system("clear");
                              break;
                           }
            case 'M':break;
            case 'E':if(linuser_group > registered_user){
                            string nttitle;
                            cout << "Please enter the new title of the thread" << endl;
                            getline(cin, nttitle);
                            tli->set_thread_title(nttitle);
                            system("clear");
                            break;
                         }
                         else{
                              cout << "Invalid command ! Please type a valid command." << endl;
                              sleep(2);
                              system("clear");
                              break;
                        }
            case 'Y':if(linuser_group > registered_user){
                            if(tSticky){
                              thread_list.push_back(*tli);
                              sticky_thread_list.erase(tli);
                              tli = thread_list.end();
                              tli--;
                              system("clear");
                              break;
                            }
                            else{
                              sticky_thread_list.push_back(*tli);
                              thread_list.erase(tli);
                              tli = sticky_thread_list.end();
                              tli--;
                              system("clear");
                              break;
                            }
                         }
                         else{
                              cout << "Invalid command ! Please type a valid command." << endl;
                              sleep(2);
                              system("clear");
                              break;
                        }
            case 'K':if(linuser_group > registered_user){
                              tli->toggleLock();
                              system("clear");
                              break;
                         }
                         else{
                              cout << "Invalid command ! Please type a valid command." << endl;
                              sleep(2);
                              system("clear");
                              break;
                        }
            case 'A':if(linuser_group > registered_user){
                           string pid;
                           cout << "Please enter the ID of the post you want to delete" << endl;
                           getline(cin, pid);
                           if(pid.size() > 3 || pid.size() == 0){
                              cout << "Invalid command ! Please type a valid command." << endl;
                              sleep(2);
                              system("clear");
                              break;
                           }
                           int enteredID = usefunc.string2int(pid);
                           if(enteredID == -1){
                              cout << "Invalid command ! Please type a valid command." << endl;
                              sleep(2);
                              system("clear");
                              break;
                           }
                           tli->deletePost(enteredID);
                           system("clear");
                           break;
                         }
                         else{
                              cout << "Invalid command ! Please type a valid command." << endl;
                              sleep(2);
                              system("clear");
                              break;
                        }
            case 'S':
            case 'L':if(linuser_group > guest){
                           nInfo.navi = exitProgramSCS;
                           system("clear");
                           return nInfo;
                           }
                           else{
                              cout << "Invalid command ! Please type a valid command." << endl;
                              sleep(2);
                              system("clear");
                              break;
                           }
            case 'X':nInfo.navi = exitProgram;
                           system("clear");
                           return nInfo;
            default:cout << "Invalid command ! Please type a valid command." << endl;
                        sleep(2);
                        system("clear");
                        break;
         }
      }
   }
   else{
      cout << "\nThe ID entered does not correspond to an existing thread ! " << endl;
      sleep(2);
      system("clear");
      nInfo.navi = nop;
      return nInfo;
   }
}

list<forum> ::iterator forum::get_fli(eiPosition ipos){
   switch(ipos){
      case beginning: return forum_list.begin();
      case end : return forum_list.end();
   }
}

void forum::showAvThreads(){
   list<thread>::iterator tli;
   for(tli = sticky_thread_list.begin(); tli != sticky_thread_list.end(); tli++){
      cout << "Sticky: " + tli->get_thread_title() + ", created by " + tli->get_creator_username() + " (ID: " << tli->get_thread_ID() << ")"<< endl;
   }
   for(tli = thread_list.begin(); tli != thread_list.end(); tli++){
      cout << tli->get_thread_title() + ", created by " + tli->get_creator_username() + " (ID: " << tli->get_thread_ID() << ")"<< endl;
   }
}

 void forum::set_forum_title(const string& nftitle){
    forum_title = nftitle;
 }

void forum::createSubforum(const string& title, int fID){
   list<forum>:: iterator fli;
   forum *fptr;
   fptr = new forum;
   forum_list.push_back(*fptr);
   delete fptr;
   fli = forum_list.end();
   fli--;
   fptr = new forum(title, fID, selfi, fli);
   (*fli) = *fptr;
   delete fptr;
}

 void forum::addThread(const string& ntTitle, int tID, bool lcd, const string& cusrname, const string& pcont, int pID){
    thread nt(ntTitle, tID, cusrname, lcd);
    nt.addPost(pcont, pID, cusrname);
    thread_list.push_back(nt);
 }

bool forum::isUnderSystem(){
   return underSystem;
}
void forum::deleteForum(list<forum>::iterator pos){
      forum_list.erase(pos);
}
forum::forum(const string& title, int fID, const list<forum> ::iterator& sfi){
   forum_title = title;
   forum_ID = fID;
   selfi = sfi;
   underSystem = true;
}
forum::forum(const string& title, int fID, const list<forum> ::iterator& pfi, const list<forum> ::iterator& sfi){
   forum_title = title;
   forum_ID = fID;
   selfi = sfi;
   parenti = pfi;
   underSystem = false;
}
forum::forum(){}
