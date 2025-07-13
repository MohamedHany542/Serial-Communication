# ifndef LCD_H
# define LCD_H

using namespace std;

class LCD{
   private:
   int RS, RW, E, d7, d6, d5, d4,
       RS_u, RW_u, E_u, d7_u, d6_u, d5_u, d4_u;
   bool mode;
   void writelcd();
   void chkbf();
   int bmap(int add_d);

   public:
   LCD(int RS_user, int RW_user, int E_user, int d7_user, int d6_user, int d5_user, int d4_user, bool mode_user);
   void init();
   void setaddress(int row, int column);
   void print(char message[]);
   void cursor();
   void nocursor();
};

#endif