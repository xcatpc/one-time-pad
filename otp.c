/*

-- TO DO --
# how big is the file
# message if not enought random_numbers are available

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef __unix__
   #include <sys/stat.h>
   #include <sys/types.h>
   #define clear system("clear")
   #define SLEEP sleep
   #define SEK 3
#elif __WIN32__ || _MSC_VER
   #include <sys\stat.h>
   #include <windows.h>
   #define clear system("cls")
   #define SLEEP Sleep
   #define SEK 3000
#elif __APPLE__
#include <sys/stat.h>
   #include <sys/types.h>
   #include <unistd.h>

   #define clear system("clear")
   #define SLEEP sleep
   #define SEK 3

#endif

// PROTOTYPES  /////////////////////////////////////////////////////////////////////////////////////////////////
int print_logo();
int print_syntax();
int print_menu1();
int print_sig_menu();

int sig_key(int modus, char *n_trn_key, char *n_sig_key);
int encryption(char *n_unver_quelldatei, char *n_schluessel, char *n_verschl_dateiname);
int decryption(char *n_ver_quelldatei, char *n_schluessel, char *n_unver_dateiname);

char *FILE_RAM(char *n_datei);
char *sig_extraction(char *datei);
char *vevn_extraktion(char *datei);
unsigned long used_random_numbers(char *datei, int rauten);

unsigned long z_key_vvv;
unsigned long z_key_nnn; //TRN FILE
unsigned long z_file_nnn;


// FUNCTIONS /////////////////////////////////////////////////////////////////////////////////////////////////
int print_logo()
{
  //clear;
  printf("##########################################################\n");
  printf("####      One Time Pad - most secure encryption       ####\n");
  printf("####   (C)2000 by xcatpc    |      xcatpc@proton.me   ####\n");
  printf("##########################################################\n\n");
  return 0;
}

int print_syntax()
{
  printf("-----------------------------------------------------------\n");
  printf(" : otp -v filename key enc_filename\n");
  printf(" : otp -e enc_file key filename\n");
  printf(" : otp -s [modedus (1-3)] true_random_key_file keyname (e.g.: Name_0001)\n"); 
  printf("-----------------------------------------------------------\n\n");
  printf("#### mode:                                     ####\n");
  printf("#### (1) create encyption key from true random number file ####\n");
  printf("#### (2) create decryption key from true random number file ####\n");
  printf("#### (3) create both   ####\n\n");
  return 0;
}

int print_menu1()
{
  printf("(1) create keys out of true random number file\n");
  printf("(2) encrypt file\n");
  printf("(3) decrypt file\n");
  printf("(4) quit\n");
  printf("\n");
  printf("choise (number): ");
  return 0;
}

int print_sig_menu()
{
  printf("(1) create encyption key from true random number file\n");
  printf("(2) create decryption key from true random number file\n");
  printf("(3) create both\n");
  printf("(4) quit\n");
  printf("\n");
  printf("choise (number): ");
  return 0;
}

int sig_key(int modus, char *n_trn_key, char *n_sig_key)
{
  int x;
  printf("True random file is loading into RAM memory...\n");
  char *trn_key = FILE_RAM(n_trn_key);
  char buffer[300];
  
  if(strcmp(sig_extraction(trn_key), "NNN") != 0) // proof if signature exists
  {
    printf("ERROR! TRN file has a signature\n");
    return 1;
  }
  
  if(modus == 1 || modus == 3) // encrypt
  {
    printf("encryption key will be generated...\n");
    sprintf(buffer, "%s_%ldMB_VVV.zfz", n_sig_key, z_key_nnn/1000000);
    FILE *key_vvv = fopen(buffer, "w+b");
    if(key_vvv == NULL)
    {
      printf("ERROR! key VVV not created!\n");
      return 1;
    }
    sprintf(buffer, "#####%s_##0#VVV#####", n_sig_key);
    fputs(buffer, key_vvv);
    for(x=0; x<=z_key_nnn-1; x++) //FEHLER!!!!!!! SPEICHERZUGRIFFSFEHLER
    putc(trn_key[x], key_vvv);
    fclose(key_vvv);
    printf("# encryption key created successfully!\n");
  }
  
  if(modus == 2 || modus == 3) // decrypt
  {
    printf("decryption key will be generated...\n");
    sprintf(buffer, "%s_%ldMB_EEE.zfz", n_sig_key, z_key_nnn/1000000);
    FILE *schluessel_eee = fopen(buffer, "w+b");
    if(schluessel_eee == NULL)
    {
      printf("ERROR! key VVV not created!\n");
      return 1;
    }
    sprintf(buffer, "#####%s_##0#EEE#####", n_sig_key);
    fputs(buffer, schluessel_eee);
    
    for(x=0; x<=z_key_nnn-1; x++)
    putc(trn_key[x], schluessel_eee);
    fclose(schluessel_eee);
    printf("# decryption key created successfully!\n");
  }
  free(trn_key);
  return 0;
}

int encryption(char *n_unver_quelldatei, char *n_schluessel, char *n_verschl_dateiname)
{
  printf("encryption key will is loading into memory...\n");
  char *key_vvv = FILE_RAM(n_schluessel);
  printf("file will is loading into memory...\n");
  char *quelldatei_nnn = FILE_RAM(n_unver_quelldatei);
  char buffer[300];
  unsigned long tmp_used_trn = used_random_numbers(key_vvv, 7);
  unsigned long tmp_sig_key_lengh = strlen(sig_extraction(key_vvv));
  int x;
  
  if(strcmp(sig_extraction(quelldatei_nnn), "NNN") != 0) // Kontrolle ob Signatur vorhanden
  {
    printf("ERROR! file has a signature!\n");
    return 1;
  }
  
  if(strcmp(vevn_extraktion(key_vvv), "VVV") != 0)
  {
    printf("ERROR! key is not an encryption key!\n");
    return 1;
  }
  
  FILE *zieldatei_ver = fopen(n_verschl_dateiname, "w+b");
  if(zieldatei_ver == NULL)
  {
    printf("ERROR! enc file not created!\n");
    return 1;
  }
  
  //SIGNATUR IN DIE VERSCHLÜSSELTE DATEI SCHREIBEN
  sprintf(buffer, "#####%s#%ld#%ld#VER#####", n_schluessel, tmp_used_trn, tmp_used_trn + z_file_nnn);

  //buffer in datei schreiben
  fputs(buffer, zieldatei_ver);
  
  for(x=0; x<=z_file_nnn; x++)
  {
    putc((quelldatei_nnn[x] ^ key_vvv[x + tmp_used_trn + tmp_sig_key_lengh]), zieldatei_ver);
  }
  printf("\n# File encrypted successfully.\n");
  printf("Used true random numbers will be deducted...\n");
 

  //NEUEN VERSCHLÜSSELUNGSSCHLÜSSEL ERSTELLEN
  FILE *schluessel_neu = fopen("temp", "w+b");
  if(schluessel_neu == NULL)
  {
    printf("ERROR! key file not created!\n");
    return 0;
  }

  sprintf(buffer, "#####%s##%ld#VVV#####", n_schluessel, z_file_nnn + used_random_numbers(key_vvv, 7));
  //buffer in datei schreiben
  fputs(buffer, schluessel_neu);

  //Verwendete Zufallszahlen durch '0' ersetzen
  for(x=1; x<=(tmp_used_trn + z_file_nnn); x++)
  {
    putc('0', schluessel_neu);
  }

  for(x=0; x<=z_key_vvv - (tmp_used_trn + z_file_nnn + tmp_sig_key_lengh)-1; x++)
  {
    putc(key_vvv[x+tmp_used_trn + z_file_nnn + tmp_sig_key_lengh], schluessel_neu);
  }

  fclose(schluessel_neu);
  fclose(zieldatei_ver);
  free(key_vvv);
  free(quelldatei_nnn);
  remove(n_schluessel);
  rename("temp", n_schluessel);  

  return 0;
}

int decryption(char *n_ver_quelldatei, char *n_schluessel, char *n_unver_dateiname)
{
  printf("encryption key loading into the ram memory...\n");
  char *schluessel_eee = FILE_RAM(n_schluessel);
  printf("file will be loading into the ram memory...\n");
  char *quelldatei_ver = FILE_RAM(n_ver_quelldatei);
  char *signatur_quelldatei_ver = sig_extraction(quelldatei_ver);
  char *signatur_schluessel_eee = sig_extraction(schluessel_eee);
  char buffer[300];
  char buffer2[300];
  unsigned int x, y, z;
  unsigned long zfz_anfang, zfz_ende;
  
  if(strcmp(vevn_extraktion(schluessel_eee), "EEE") != 0)
  {
    printf("ERROR! file is not a encryption key!\n");

    return 1;
  }
  
  if(strcmp(vevn_extraktion(quelldatei_ver), "VER") != 0)
  {
    printf("ERROR! Its not a encrypted file!\n");
    return 1;
  }
  
  ///////////////////////////////////////////////////////////////////////////////////////////////////////
  //ÜBERPRÜFEN OB RICHTIGER SCHLÜSSEL GEWÄLT
  //printf("quelldatei signatur = %s\n", signatur_quelldatei_ver);
  y=0;
  z=0;
  for(x=5; x<=strlen(signatur_quelldatei_ver); x++)
  {
    if(signatur_quelldatei_ver[x] == '_')
      y++;
    if(y<=1)
    {
      buffer[x-5] = signatur_quelldatei_ver[x];
      z++;
    }
  }
  buffer[z] = '\0';
  
  y=0;
  z=0;
  for(x=5; x<=strlen(signatur_schluessel_eee); x++)
  {
    if(signatur_schluessel_eee[x] == '_')
      y++;
    if(y<=1)
    {
      buffer2[x-5] = signatur_schluessel_eee[x];
      z++;
    }
  }
  buffer2[z] = '\0';
  //printf("buffer = %s\n", buffer);
  //printf("buffer2 = %s\n", buffer2);
  
  if(strcmp(buffer, buffer2) == 0)
  {
    printf("encryption key match!\n");
  }
  else
  {
    printf("file was encrypted with another key!\n");
    //return 0;
  }
  //ENTSCHLÜSSELTE DATEI ERSTELLEN (OHNE Signatur)
  FILE *entschluesselte_datei = fopen(n_unver_dateiname, "w+b");
  if(entschluesselte_datei == NULL)
  {
    printf("ERROR! new key file not created!\n");
    return 0;
  }
  //Benoetigte ZFZ-Stelle extrahieren (Bsp.: 50-100)
  zfz_anfang = used_random_numbers(quelldatei_ver, 6);
  zfz_ende = used_random_numbers(quelldatei_ver, 7);
  
  /*printf("zfz_anfang: %ld\n", zfz_anfang);
  printf("zfz_ende: %ld\n", zfz_ende); */
  for(x=0; x<=(zfz_ende-zfz_anfang)-1; x++)
  {
    putc(quelldatei_ver[x + strlen(signatur_quelldatei_ver)] ^ schluessel_eee[x + zfz_anfang + strlen(signatur_schluessel_eee)], entschluesselte_datei); 
  }
  fclose(entschluesselte_datei);
  free(schluessel_eee);
  free(quelldatei_ver);
  printf("# file encrypted successfully!\n");
  return 0;
}

char *FILE_RAM(char *n_datei)
{
  struct stat attribut;
  unsigned long z_datei, x;
  char *buffer;
  unsigned int c;
  
  FILE *datei = fopen(n_datei, "r+b");
  if(datei == NULL)
  {
    printf("File: %s can't be opened!\n", n_datei);
    return "1";
  }
  
  else
  {
    if(stat(n_datei, &attribut) == -1)
    {
      printf("Error\n");
      return "1";
    }
    z_datei = attribut.st_size;
  }
  //printf("z_datei = %d\n", z_datei);

  buffer = malloc(z_datei * sizeof(char)+1);
  if(buffer == NULL)
  {
    printf("Error at malloc\n");
    return "1";
  }
  
  for(x=0; x<=z_datei; x++)
  {
    c=getc(datei);
    buffer[x] = c;
  }
  fclose(datei);

  if(strcmp(vevn_extraktion(buffer), "VVV") == 0 )
  {
    //printf("buffer: %s == VVV\n", n_datei);
    z_key_vvv = z_datei;
  }
  else if(strcmp(vevn_extraktion(buffer), "EEE") == 0) 
  {
    //printf("buffer: %s == EEE\n", n_datei);
  }
  else if(strcmp(vevn_extraktion(buffer), "VER") == 0)
  { 
    //printf("buffer: %s == VER\n", n_datei);
  }
  else
  {
    //printf("buffer: %s == NNN\n", n_datei);
    z_key_nnn = z_datei;
    z_file_nnn = z_datei;
  }
  
  return buffer;
}

char *sig_extraction(char *datei)
{
  char *signatur;
  int x, y;
  
  for(x=0; x<=4; x++)
  {
    if(datei[x] != '#')
      return "NNN";
  }

  y=0;
  for(x=0; x<=strlen(datei); x++)
  {
    if(datei[x] == '#') { y++; }
    if(y >= 13)
      break;
  }
  
  signatur = (char *)malloc(x+1 * sizeof(char));
  if(signatur == NULL)
  {
    printf("Error at malloc\n");
    return "1";
  }
  for(y=0; y<=x; y++)
    signatur[y] = datei[y];
  signatur[y] = '\0';
  
  return signatur;
}

char *vevn_extraktion(char *datei)
{
  char *vevn = malloc(sizeof(char) * 4);
  char *schluessel_vvv_signatur = sig_extraction(datei);
  vevn[2] = schluessel_vvv_signatur[strlen(schluessel_vvv_signatur)-6];
  vevn[1] = schluessel_vvv_signatur[strlen(schluessel_vvv_signatur)-7];
  vevn[0] = schluessel_vvv_signatur[strlen(schluessel_vvv_signatur)-8];
  vevn[3] = '\0';
  //printf("%s\n", vevn);
  return vevn;
}

unsigned long used_random_numbers(char *datei, int rauten)
{
  char *schluessel_vvv_signatur = sig_extraction(datei);
  char buffer[300];
  int x, y, z, i_temp;
 
  z=0;
  i_temp = 0;
  x=strlen(schluessel_vvv_signatur);
  for(y=0; y<=x; y++)
  {
    if(schluessel_vvv_signatur[y] == '#') { z++; }
    if(z == rauten)
    {
      if(schluessel_vvv_signatur[y] == '#') {}
      else
      {
	buffer[i_temp] = schluessel_vvv_signatur[y];
	i_temp++;
      }
    }
  }
  return atof(buffer);
}


// HAUPTPROGRAMM
int main(int argc, char **argv)
{
  if(argc < 2)
  {
    int auswahl;
    while(1)
    {
      print_logo();
      print_syntax();
      print_menu1();
      do {scanf("%i", &auswahl);} while ( getchar() != '\n' );

      if(auswahl == 1) //sig_key
      {
	int modus;
	char rohschluessel[200];
	char schluesselname[200];
	print_sig_menu();
	do {scanf("%i", &modus);} while ( getchar() != '\n' );
	if(modus == 1 || modus == 2 || modus == 3) {}
	else
	{
	  printf("mode only between 1 - 3\n");
	  printf("1 - encryption key\n");
	  printf("2 - decryption key\n");
	  printf("3 - create both keys\n");
	  return 0;
        }
        
        printf("Please type in your true_random_number_filename: ");
	do {scanf("%200s", rohschluessel);} while ( getchar() != '\n' );
	
	printf("Type in a key_name (e.g.: Person_0001): ");
	do {scanf("%200s", schluesselname);} while ( getchar() != '\n' );
        
        //sig_key(modus, rohschluessel, schluesselname);
        sig_key(modus, rohschluessel, schluesselname);
      }

      else if(auswahl == 2) //encryption
      {
	char unver_quelldatei[200];
	char key[200];
	char ver_dateiname[200];
	
        printf("Which file should be encrypted?: ");
	do {scanf("%200s", unver_quelldatei);} while ( getchar() != '\n' );
	
	printf("Please type in your encryption key: ");
	do {scanf("%200s", key);} while ( getchar() != '\n' );
	
	printf("Name of the encrypted file: (e.g. filename_typ.otp): ");
	do {scanf("%200s", ver_dateiname);} while ( getchar() != '\n' );
	
	//encryption(filename, key, enc_filename);
	encryption(unver_quelldatei, key, ver_dateiname);
      }

      else if(auswahl == 3) //decryption
      {
	char ver_dateiname[200];
	char key[200];
	char unver_dateiname[200];
	
	printf("Which file should be encrypted?: ");
	do {scanf("%200s", ver_dateiname);} while ( getchar() != '\n' );
	
	printf("Please type in your encryption key: ");
	do {scanf("%200s", key);} while ( getchar() != '\n' );
	
	printf("Name of the encrypted file: (e.g. filename_typ.otp): ");
	do {scanf("%200s", unver_dateiname);} while ( getchar() != '\n' );
	
	//decryption(enc_filename, key, unverschl_Dateiname);
	decryption(ver_dateiname, key, unver_dateiname);
      }

      else if(auswahl == 4)
      {
        printf("quit application...\n");
        return 0;
      }
      else
        printf("only mode between 1-4\n");
      SLEEP(SEK);
    }
  }
  
// KONSOLENSTART ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  else if(argc >= 2)
  {
    if( strcmp(argv[1], "-v") == 0 && argc == 5) //encryption
    {
      //encryption(filename, key, enc_filename);
      encryption(argv[2], argv[3], argv[4]);
    }
    
    else if( strcmp(argv[1], "-e") == 0 && argc == 5) //decryption
    {
      //decryption(enc_filename, key, unverschl_Dateiname);
      decryption(argv[2], argv[3], argv[4]);
    }
    
    else if (strcmp(argv[1], "-s") == 0 && argc == 5) //sig_key
    {
      int modus = atoi(argv[2]);
      if(modus <=0 || modus >3)
      {
	printf("modes only from 1-3\n");
	printf("1 - encryption key\n");
	printf("2 - decryption key\n");
	printf("3 - create both keys\n");
	return 0;
      }
      
      //sig_key(modus, rohschluesse, schluesselname);
      sig_key(modus, argv[3], argv[4]);
    }
    
    else
    {
      print_logo();
      print_syntax();
      printf("Wrong command! -> look at Syntax\n");
      return 0;
    }
  }
  
  return 0;
}
