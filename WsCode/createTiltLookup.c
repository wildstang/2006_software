#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OUTPUT_FOR_RC 1

void print_header(void);
void print_value(int, int);

int cam_range;

int main(int argc, char *argv[])
{
  int i = 0;
  int x = 0;

  int known_range;
  float mu;
  int y0, y1;
  int pos = 0;
  float value;

  int cam_bot;
  int cam_top;
  int pot_top;

  FILE *fp;
  char line[100];
  char *part = NULL;
  char *ret;
  int cam_vals[100];
  int pot_vals[100];
  int temp_val;
  int num_vals = 0;


  if(argc < 2)
  {
    printf("Usage: createTiltLookup pot_top");
    exit(0);
  }

  /* Store the pot value at the bottom */
  pot_top = atoi(argv[1]);

  /* Open a the file that contains the known points.
   * The program allows comments to be placed using
   * a # as the first character on the line.
   * Lines containing data points should have the format
   * 
   * CAM_VALUE POT_VALUE # Optional comment
   */
  fp = fopen("tiltLookupVals.txt", "r");
  while(1)
  {
    /* Get a line from the file */
    ret = fgets(line, 100, fp);
    if(ret == NULL)
    {
      /* Jump out of the while loop
       * if no line is returned */
      break;
    }
    else
    {
      if(!strncmp("#", line, 1))
      {
        /* We got a comment...go to the next line */
        continue;
      }

      /* Get the first space delimited field */
      part = (char *) strtok(line, " " );
      /* Convert it to an integer */
      temp_val = atoi(part);
      if(temp_val == 0)
      {
        /* Go to the next line, the line is blank */
        continue;
      }
      /* Store the field in the cam values */
      cam_vals[num_vals] = temp_val;

      /* Get the next field and store it
       * in the pot vals */
      part = ((char *)strtok(NULL, " "));
      pot_vals[num_vals]  = atoi(part);
    }

    num_vals++;
  }
  fclose(fp);


  /* Store off camera range information */
  cam_bot = cam_vals[0];
  cam_top = cam_vals[num_vals - 1];
  cam_range = cam_top - cam_bot;

  /* Pringt the header */
  print_header();

  /* Loop over the camera range */
  for(i = 0; i <= cam_range; i++)
  {
    if(i > (cam_vals[pos + 1] - cam_bot))
    {
      /* The current index is outside the range that
       * can be processed.  Increment so that we're in
       * the next range */
      pos++;
      x = 1;
    }

    /* Do a linear regression between the current cam value
     * and the next to get the next value */
    known_range = (cam_vals[pos+1] - cam_bot) - (cam_vals[pos] - cam_bot);
    /* We want mu to be 0 at the first point, 1 at the second, and
     * somewhere in between otherwise */
    mu = x / (float) known_range;
    y0 = pot_vals[pos];
    y1 = pot_vals[pos+1];
    value = (float) (y0 * (1 - mu)) + (float) (y1 * mu);
    x++;

    /*
    printf("%d %d %d\n", (int)value, pot_top, (int)(value) - pot_top);
    */
    /* Print the value out */
    //print_value(i, (int)(value) - pot_top);
    print_value(i, round((value) - pot_top));
  }
  printf("\n");

  return 0;
}

void print_header()
{
  if (OUTPUT_FOR_RC == 1)
  {
    printf("const rom UINT16 tilt_lookup[] = \n");
    printf("/*          0    1    2    3    4    5    6    7    8    9 */\n");
  }
}

void print_value(int i, int value)
{
  static int out_col = 0;
  static int out_line = 0;

  if (OUTPUT_FOR_RC == 1)
  {
    if(out_col == 0)
    {
      printf("/* %d */  ", out_line);
      if(out_line != 0)
      {
        printf(" ");
      }
    }
    if (i == 0)
    {
      printf("{");
    }
    printf("%3d", value);

    if(i != (cam_range))
    {
      printf(", ");
    }
    else
    {
      printf("};");
    }

    if(out_col == 9)
    {
      printf("\n");
      out_col = 0;
      out_line++;
    }
    else
    {
      out_col++;
    }
  }
  else
  {
    printf("%d, %d\n", i, value);
  }
}
