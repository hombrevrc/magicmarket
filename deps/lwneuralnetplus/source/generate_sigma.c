/* generate_sigma.c -- generate interpolation table for sigmoidal function
 * copyright (c) 2003 Peter van Rossum
 * Adapted for lwneuralnet++
 */

#include <math.h>
#include <stdio.h>
#include <getopt.h>


/****************************************
 * Settable parameters
 ****************************************/

float min_entry = -13.2;        /* first entry in the table of logistic function, switch -f */
float max_entry = 13.2;         /* last entry in the table of logistic function, switch -l */
float min_entry_tanh = -3.13;     /* first entry in the table of tanh functioon, switch -t */
float max_entry_tanh = 3.13;      /* last entry in the table of tanh function, switch -u */
int num_entries = 2000;         /* number of entries in the table, switch -n */

int
parse (int argc, char *argv[])
{
  int c;
  while ((c = getopt (argc, argv, "f:l:n:h:t:u:")) != -1) {
    switch (c) {
    case 'h':
      printf ("Usage: generate_sigma [OPTIONS]\n");
      printf
        ("Generate an interpolation table for s(x) = 1/(1+exp(-x)) and s(x)=tanh(x)\n\n");
      printf ("  -f first_entry_of_table for logistic function\n");
      printf ("  -l last_entry_of_table for logistic function\n");
      printf("   -t first_entry_of_table for tanh function\n");
      printf ("  -u last_entry_of_table for tanh function\n");
      printf ("  -n number_of_entries_in_table\n");
      return 0;
      break;
    case 'f':
      if (sscanf (optarg, "%f", &min_entry) == 0)
        return 0;
      break;
    case 'l':
      if (sscanf (optarg, "%f", &max_entry) == 0)
        return 0;
      break;
    case 't' :
      if (sscanf (optarg, "%f", &min_entry_tanh) == 0)
	return 0;
      break;
    case 'u' :
      if (sscanf (optarg, "%f", &max_entry_tanh) == 0)
	return 0;
      break;
    case 'n':
      if (sscanf (optarg, "%i", &num_entries) == 0)
        return 0;
      break;
    }
  }
  return 1;
}

/****************************************
 * Main
 ****************************************/

int
main (int argc, char *argv[])
{
  int i;
  float xl, xt, yl ,yt;
  float interval, interval_tanh;

  if (parse (argc, argv) == 0)
    return 0;

   interval = (max_entry - min_entry) / (num_entries - 1);
   interval_tanh = (max_entry_tanh - min_entry_tanh) / (num_entries - 1);

  printf ("/* automatically generated by generate_sigma */\n");
  printf ("static const float min_entry[2] = {%.20f, %.20f};\n", min_entry,min_entry_tanh);
  printf ("static const float max_entry[2] = {%.20f, %.20f};\n", max_entry, max_entry_tanh);
  printf ("static const int num_entries = %i;\n", num_entries);
  printf ("static const float invinterval[2] = {%.20f, %.20f};\n", 1.0/interval, 1.0/interval_tanh);
  printf("static const float lowbound[2] = { 0.0, -1.0 }; \n");
  printf("static const float highbound[2] = { 1.0, 1.0 }; \n");
  printf ("static const float interpolation [%i][2] = {\n", num_entries);
  for (i = 0; i < num_entries; i++) {
    xl = interval * i + min_entry;
    xt = interval_tanh * i + min_entry_tanh;
    yl = 1 / (1 + exp (-xl));
    yt = tanh(xt);
    printf ("{  %.20f, %.20f}%s\n", yl, yt, i == num_entries - 1 ? "" : ",");
  }
  printf ("};\n");


  return 0;
}




