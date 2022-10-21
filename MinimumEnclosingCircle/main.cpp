#include <stdio.h>
#include <graphics.h>
#include <sstream>
#include <math.h>
#include <time.h>

struct Point
{
    double X,Y;
};
struct Circle
{
    Point C;
    double R;
};
void swap (Point *a, Point *b)
{//helper function for shuffle function
    Point temp = *a;
    *a = *b;
    *b = temp;
}
void shuffle ( Point arr[], int n )
{   //function to shuffle an array
    srand ( time(NULL) );

    for (int i = n-1; i > 0; i--)
    {
        int j = rand() % (i+1);

        swap(&arr[i], &arr[j]);
    }
}

double distance_between_points(Point A, Point B)
{   //function for finding the distance between two points
    return sqrt(pow(A.X - B.X, 2) + pow(A.Y - B.Y, 2));
}
bool is_inside_circle(Circle C, Point P)
{   //function to check if a point is inside a circle
    return distance_between_points(C.C, P) <= C.R;
}

Circle get_circle_3P(Point p1, Point p2, Point p3){
    //function to calculate the minimum enclosing circle for 3 points
    double p1x = p1.X, p1y = p1.Y, p2x = p2.X, p2y = p2.Y, p3x = p3.X, p3y = p3.Y;
    double a = p2x - p1x, b = p2y - p1y, c = p3x - p1x, d = p3y - p1y;
    double e = a * (p2x + p1x) / 2 + b * (p2y + p1y) / 2;
    double f = c * (p3x + p1x) / 2 + d * (p3y + p1y) / 2;
    double determinant = a * d - b * c;
    double cx = (d * e - b * f) / determinant;
    double cy = (-c * e + a * f) / determinant;
    Point P;
    P.X = cx;
    P.Y = cy;
    double R = sqrt((p1x - cx) * (p1x - cx) + (p1y - cy) * (p1y - cy));
    return {P,R};
}
Circle get_circle_2P(Point p1, Point p2){
    //function to calculate the minimum enclosing circle for 2 points
    double p1x = p1.X, p1y = p1.Y, p2x = p2.X, p2y = p2.Y;
    double cx = 0.5 * (p1x + p2x);
    double cy = 0.5 * (p1y + p2y);
    Point P;
    P.X = cx;
    P.Y = cy;
    double R = sqrt((p1x - cx) * (p1x - cx) + (p1y - cy) * (p1y - cy));
    return {P,R};


}

Circle MEC(Point p[],int n, Point B[],int b)
{   //implementation of algorithm
    Circle C = {};
   if(b==3){
        C = get_circle_3P(B[0],B[1],B[2]);
    }
    else if(n == 1 && b == 0){
        C.C.X = p[0].X;
        C.C.Y = p[0].Y;
        C.R = 0;
    }
    else if (n == 0 && b == 2){
        C = get_circle_2P(B[0],B[1]);
    }
    else if (n == 1 && b == 1){
        C = get_circle_2P(B[0],p[0]);
    }
    else {
        C = MEC(p, n - 1, B, b);
        if (!is_inside_circle(C,p[n-1])) {
            B[b++] = p[n - 1];
            C = MEC(p, n - 1, B, b);
        }
    }
    return C;

}

int main()
{
    printf("Girilen koordinatlar\n\n");
    initwindow(800,800);
    line(0,getmaxy()/2,800,getmaxy()/2);//x-axis itself
    line(getmaxx()/2,0,getmaxx()/2,800);//y-axis itself
    char x_short_line[20],y_short_line[20];
    int x_coordinate_numbers = -24,y_coordinate_numbers = 24;
    //x-axis short lines, long lines and numbers
    for(int i=16; i<800; i=i+16)
    {
        if(i!=400)
        {
            setlinestyle(0,0,2);
            line(i-1,395,i-1,405);
            setlinestyle(0,0,0);
            setcolor(8);
            line(i-1,0,i-1,getmaxy());
            setcolor(15);

        }
        sprintf(x_short_line,"%d",x_coordinate_numbers);
        if(i!=400)//in order to not to write 0
        {
            outtextxy(i-7,410,x_short_line);
        }

        x_coordinate_numbers += 1;
    }
    //y-axis short lines, long lines and numbers
    for(int i=16; i<800; i=i+16)
    {
        if(i!=400)
        {
            setlinestyle(0,0,2);
            line(395,i-1,405,i-1);
            setlinestyle(0,0,0);
            setcolor(8);
            line(0,i-1,getmaxx(),i-1);
            setcolor(15);
        }

        sprintf(y_short_line,"%d",y_coordinate_numbers);
        if(i!=400)//in order to not to write 0
        {
            outtextxy(410,i-7,y_short_line);
        }


        y_coordinate_numbers-= 1;
    }

    FILE *fp;

    fp = fopen("coordinates.txt","r");
    if(fp == NULL)
    {
        printf("Dosya bulunamadý");
    }
    else
    {
        int line_count = 0;
        char control;
        while(!feof(fp))
        {
            control=fgetc(fp);
            if(control==10)//counting \n character
                line_count++;

        }
        line_count++;//in case if there is no \n in the last line (which is the case in my file)
        fclose(fp);

        fp = fopen("coordinates.txt","r");

        int xy[line_count*2],x[line_count],y[line_count],x_index = 0,y_index = 0;
        //passing coordinates to arrays from file
        for(int i = 0; feof(fp)<1; i++)
        {
            fscanf(fp, "%d", &xy[i]);
        }
        for(int i = 0; i<line_count*2; i++)
        {
            if(i%2==0)
            {
                x[x_index] = xy[i];
                x_index +=1;
            }
            else
            {
                y[y_index] = xy[i];
                y_index +=1;
            }
        }
        fclose(fp);
        //printing points
        for(int i = 0; i < line_count; i++)
        {
            setcolor(14);
            circle((x[i]+25)*16,(25-y[i])*16,4);
            floodfill((x[i]+25)*16,(25-y[i])*16,YELLOW);
            setcolor(15);
        }

        struct Point P[line_count],P_random[line_count];
        struct Point boundary_points[] = {};
        //passing points to a Point array
        fp = fopen("coordinates.txt","r");
        for(int i = 0; feof(fp)<1; i++)
        {
            fscanf(fp, "%lf %lf", &P[i].X, &P[i].Y);
        }
        fclose(fp);
        //writing coordinates of points
        for(int i = 0; i < line_count; i++)
        {
            printf("X:%f",P[i].X);
            printf(" Y:%f\n",P[i].Y);

        }
        memcpy(&P_random, &P, sizeof(P));//copying P array to P_random array
        shuffle(P_random,line_count);//shuffling the array so we don't have to pick random points in each iteration
        Circle abcd = MEC(P_random,line_count,boundary_points,0);//running the algorithm
        printf("\nMerkez X =%f Merkez Y =%f Yaricap =%f",abcd.C.X, abcd.C.Y, abcd.R);//printing circle's values
        circle((abcd.C.X+25)*16,(25 -abcd.C.Y)*16,abcd.R*16);//printing circle itself
        setcolor(4);
        circle((abcd.C.X+25)*16,(25 -abcd.C.Y)*16,4);//printing center of the circle
        setfillstyle(1,4);
        floodfill((abcd.C.X+25)*16,(25 -abcd.C.Y)*16, RED);
        line((abcd.C.X+25)*16,(25 -abcd.C.Y)*16,(abcd.C.X+25)*16,(25 -abcd.C.Y)*16 +abcd.R*16);
        fclose(fp);
        getchar();
        getch();
        return 0;
    }
}
