//Cubic B-Spline Curve
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "FPToolkit.c"

//Function to display system
void display(int m, int n, double system[m][n]) {
  printf("\n");
  for(int i = 0; i < m; ++i) {
    printf("[%d]:\t", i);
    if(system[i][0] != 0) {
      printf("%2.2lf\t", system[i][0]); 
    }
    else {
      printf("-----\t");
    }
    for(int j = 1; j < (n - 1); ++j) {
      if(system[i][j] != 0) {
        printf("%2.2lf\t", system[i][j]); 
      }
      else {
        printf("-----\t");
      }
    }
    printf("|\t%2.2lf\n", system[i][(n - 1)]);
  }
}

//Function to use gaussian elimination to solve the system
void gauss(int m, int n, double system[m][n], double var[]) {
  double temp, factor;
  //Perform Gaussian Elimination with Partial Pivoting
  for (int i = 0; i < m; ++i) {
    //Partial Pivoting: Find the row with the maximum absolute value in the current column
    int pivotRow = i;
    for (int k = i + 1; k < m; ++k) {
      if (fabs(system[k][i]) > fabs(system[pivotRow][i])) {
        pivotRow = k;
      }
    }

    //Swap rows if necessary
    if (pivotRow != i) {
      for (int j = 0; j < n; ++j) {
        temp = system[i][j];
        system[i][j] = system[pivotRow][j];
        system[pivotRow][j] = temp;
      }
    }

    //Check if the diagonal element is zero
    if (fabs(system[i][i]) < 1e-10) {
      printf("ERROR: The system has no unique solution.\n");
      return;
    }

    //Make the diagonal element 1
    double divisor = system[i][i];
    for (int j = 0; j < n; ++j) {
      system[i][j] /= divisor;
    }

    //Eliminate elements below the diagonal
    for (int k = i + 1; k < m; ++k) {
      factor = system[k][i];
      for (int j = 0; j < n; ++j) {
        system[k][j] -= factor * system[i][j];
      }
    }
  }

  //Display the upper triangular form
  //printf("\nUpper Triangular Form:");
  //display(m, n, system);

  //Back-substitution to find variable values
  //double var[m];
  for (int i = m - 1; i >= 0; --i) {
    var[i] = system[i][n - 1];
    for (int j = i + 1; j < n - 1; ++j) {
      var[i] -= system[i][j] * var[j];
    }
  }
}

//Function for a(t), b(t), c(t), and d(t) since they all have the same behavior
double ft(double t, int n, double var[]) {
  double result = 0;
  for(int i = n; i < (n + 4); ++i) {
    result += (var[i] * pow(t, (i % 4)));
  }
  return result;
}

int main(int argc, char **argv)
{
  int boolean;
  FILE *f;
  int n, i;
  double p[2],u,v;
  double x[50], y[50];
  double swidth, sheight;

  if ((argc != 3) && (argc != 4))  {
    printf("Instructions:\n");
    printf("CLICKED DATA:\n");
    printf("pgm screen_width screen_height\n");
    printf("DATA FROM A FILE:\n");
    printf("pgm screen_width screen_height filename\n");
    exit(0);
  }

  swidth = atoi(argv[1]);
  sheight = atoi(argv[2]);
  G_init_graphics(swidth, sheight);
  G_rgb (0,0,0);
  G_clear();
  G_rgb(1,1,1);

  if (argc == 4) {
    // input from file
    f = fopen(argv[3],"r");
    if (f == NULL) {
      printf("can't open file, %s\n",argv[3]);
      exit(1);
    }
    fscanf(f,"%d",&n);
    for (int i = 0; i < n; ++i) {
      fscanf(f,"%lf %lf\n",&x[i],&y[i]);
      G_fill_rectangle(x[i]-2.0,y[i]-2.0, 5.0,5.0);
    }
  }

  else {
    // input by clicking 
    G_rgb (0.8, 0.8, 0.8);
    G_fill_rectangle (0, sheight-20,  swidth,20);
    G_rgb(0,0,0);
    G_draw_string("click up here to terminate input",30,sheight-17);
    G_rgb (1,1,1);
    n = 0;
    G_wait_click(p);  u = p[0]; v = p[1];
    while (v < sheight - 20) {
      G_fill_rectangle(u-2.0,v-2.0, 5.0,5.0);
      x[n] = u; y[n] = v; ++n;
      G_wait_click(p);  u = p[0]; v = p[1];
    }
  }

  if(n < 4 || n > 50) {
    printf("(ERROR: Must have between 4 and 50 points)\n");
    exit(1);
  }

  printf("\n");
  printf("%d\n",n);
  for (int i = 0; i < n; ++i) {
      printf("%8.2lf  %8.2lf\n",x[i],y[i]);
  }

  //Getting system
  double system[16][17];
  memset(&system, 0, sizeof(system));
  system[15][16] = 1;
  for(int i = 0; i < 4; ++i) {
    for(int j = (i * 4); j < ((i + 1) * 4); ++j) {
      system[i][j] = 1; //1, 1, 1, 1
      if(j > (i * 4)) {
        system[(i + 5)][j] = (j - (i * 4)); //1, 2, 3
      }
      if(j > ((i * 4) + 1)) {
        system[(i + 10)][j] = ((j - (i * 4)) * system[(i + 5)][j - 1]); //2, 6
      }
      if(j % 4 == 0) {
        if(i != 3) {
          system[(i + 1)][j] = -1;
          system[(i + 6)][(j + 1)] = -1;
          system[(i + 11)][(j + 2)] = -2;
        }
        else {
          system[(i + 1)][j] = 1;
          system[(i + 6)][(j + 1)] = 1;
          system[(i + 11)][(j + 2)] = 2;
        }
        system[(15)][j] = 1;
      }
    }
  }
  
  double var[16];
  char ch = 'a';
  gauss(16, 17, system, var);
  for(int i = 0; i < 16; (i += 4)) {
    for(int j = i; j < (i + 4); ++j) { 
      printf("%c%d:\t%.10lf\t", (ch + (i/4)), (j % 4), var[j]);
    }
    printf("\n");
  }

  double gx, gy; //x and y value to graph
  double dx, dy; //difference between p3 and p0 of x and y
  double step; //steps of t
  //Graph:
  for(int i = 0; i < (n - 3); ++i) {
    dx = (abs(x[(i + 3)] - x[i]));
    dy = (abs(y[(i + 3)] - y[i]));
    if(dx > dy) {
      step = 1 / dx;
    }
    else {
      step = 1 / dy;
    }
    for(double t = 0; t <= 1; (t += step)) {
      gx = (ft(t, 0, var) * x[i]) + (ft(t, 4, var) * x[(i + 1)]) + (ft(t, 8, var) * x[(i + 2)]) + (ft(t, 12, var) * x[(i + 3)]);
      gy = (ft(t, 0, var) * y[i]) + (ft(t, 4, var) * y[(i + 1)]) + (ft(t, 8, var) * y[(i + 2)]) + (ft(t, 12, var) * y[(i + 3)]);
      G_fill_circle(gx, gy, 1);
    }
  }

  G_wait_key();
  return 0;
}
