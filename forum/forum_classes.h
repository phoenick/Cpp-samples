#include <iostream>
#include <string>
#include <list>
using namespace std;

enum user_group { guest = 0, registered_user, global_moderator, administrator };
enum mode { normal =false, registration };
enum eNavigationInstruction {exitProgram, exitProgramSCS, returntoSS, visitForum, nop };
enum eiPosition { beginning =false, end};

struct NavigationInformation;

class UsefulFunctions{
 public:
   int string2int(string str){   /*attention :it returns a positive number or -1 if
                                               something goes wrong */
      char c;
      int ac = 0, pten = 1; //ac :accumulator, pten :power of ten
      string::reverse_iterator ri;
      for(ri = str.rbegin(); ri != str.rend(); ri++){
         c  = *ri;
         if( c < 48 || c > 57)
            return -1;
         c -= 48;
         ac += c*pten;
         pten *= 10;
      }
      return ac;
}
   bool chkString(const string& str){
      string ::const_iterator si;
      for(si = str.begin(); si != str.end(); si++)
         if((*si) == ' ' || (*si) == '\t')
            return false;

      return true;
   }
};

class user {
 private:

   int user_ID;
   int user_group;
   string username;
   string password;
 public:
   user();
   /*this constructor is used for creating guests*/
   user(string personal_information);
   /*this constructor is used when initializing the forum_system using the .save files*/
   user(string gusername, string gpassword, int guser_ID); //g stands for given
   /*this constructor is used in the registration proccess*/
   string get_username() const;
   string get_password() const;
   int get_user_group() const;
   int get_user_ID() const;

   void set_username(const string& nuname);
   void set_password(const string& npasswd);
   void set_user_group(const int nugroup);
   void set_user_ID(const int nuID);

};

class post{
 private:
   string content;
   int post_ID;
   int sender_ID;
   string sender_username;
 public:
   post(const string& pcont, int pID, const string& susrname);
   string get_post_content();
   string get_sender_username();
   int get_post_ID();
};

class thread{
 private:
   string thread_title;
   int thread_ID;
   string creator_username;
   int creator_ID;
   bool sticky;
   bool locked;
   list<post> post_list;
 public:
   thread(const string& title, int tID, const string& cusrname, bool lcd);
   int get_thread_ID();
   string get_thread_title();
   string get_creator_username();
   bool isLocked();
   void addPost(const string& pcont, int pID, const string& susrname);
   void deletePost(int pid);
   void showPosts();

   void toggleLock();
   void set_thread_title(const string& nttitle);
};

class forum {
 private:
   string forum_title;
   int forum_ID;
   bool underSystem;
   list<forum> ::iterator selfi;
   list<forum> ::iterator parenti;
   list<forum> forum_list;
   list<thread> sticky_thread_list;
   list<thread> thread_list;

 public:
   //this constructor is  used to create forums that are not under the system
   forum(const string& title, int fID, const list<forum> ::iterator& pfi, const list<forum> ::iterator& sfi);
   //this constructor is  used to create forums that are under the system
   forum(const string& title, int fID, const list<forum> ::iterator& sfi);
   //this constructor creates
   forum();
   int get_forum_ID() const;
   string get_forum_title() const;
   bool isUnderSystem();
   list<forum> ::iterator get_fli(eiPosition ipos);
   list<forum> ::iterator get_selfi();
   list<forum> ::iterator get_parenti();
   void showAvThreads();
   NavigationInformation visitThread(int threadID, int linuser_group, int& LastPostID, const string& linusrname);
   void addThread(const string& ntTitle, int tID, bool lcd, const string& cusrname, const string& pcont, int pID);
   void createSubforum(const string& title, int fID);
   void deleteForum(list<forum>::iterator pos);

   void set_forum_title(const string& nftitle);
};

class forum_system{
 private:
   string system_title;
   list<user> user_list;
   list<forum> forum_list;
   user * logged_in_user;
   int LastUserID;
   int LastForumID;
   int LastThreadID;
   int LastPostID;
   bool findUser(const list<user>& gusr_list, int usrID, list<user> ::iterator& uli);
   NavigationInformation showStartScreen();
   NavigationInformation showForumVScreen(forum& vForum);
   void displayUsersList();
   void saveCurrentState();
 public:
   forum_system(string title);
   void showSystemInterface(bool mode);

};

struct NavigationInformation{
   eNavigationInstruction navi;
   list<forum> ::iterator vfi;
   int id;
};
