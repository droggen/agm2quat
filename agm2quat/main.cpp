#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "cmdline/cmdline.h"
#include <iostream>
#include <vector>
#include <string>
#include "fastmatread.h"

#include "mpu.h"
#include "mpu_geometry.h"

void Syntax()
{
    printf("agm2quat -if <agmfile> -of <quatfile> -c <col> -b <beta> -f <frq> -mds\n");
    printf("\tagmfile: text file with at least 9 consecutive columns including acceleration, gyro, magnetic field\n");
    printf("\tquatfile: Output file with quaternions\n");
    printf("\tcol: Column number (0-based) where the first of ax,ay,az,gx,gy,gz,mx,my,mz is located\n");
    printf("\tfrq: Sample rate in herzt\n");
    printf("\tmds: Magnetic downsample - specify this parameter to emulate BlueSense2 behaviour.\n");

}

/*
        data = *(MPUMOTIONDATA*)&mpu_data[mpu_data_rdptr];
        //data = mpu_data[mpu_data_rdptr];
        // Increment the read pointer
        _mpu_data_rdnext();
    }
    // Compute the geometry
    mpu_compute_geometry(data,geometry);

*/


int main(int argc,char *argv[])
{
    unsigned int channel;
    double beta = 0.35;
    unsigned frequency;
    bool mds;


    char * cwd = getcwd(0,0);
    printf("CWD: %s\n",cwd);

// Command line

    std::vector<std::string> params;
    int rv = ScanCommandLine2("-if@ -of@ -c@ -b@ -f@ -mds",argc,argv,params);
    if(rv )
    {
        Syntax();
        return 1;
    }
    printf("Input file: %s\n",params[0].c_str());
    printf("Output file: %s\n",params[1].c_str());
    printf("Channel: %s\n",params[2].c_str());
    printf("Beta: %s\n",params[3].c_str());
    printf("Sample rate: %s\n",params[4].c_str());
    mds = params[5]=="1"?1:0;
    printf("Magnetic downsample: %d\n",mds);

    if(sscanf(params[2].c_str(),"%u",&channel) != 1)
    {
        printf("Invalid specification of the agm channels\n");
        Syntax();
        return 1;
    }
    if(sscanf(params[4].c_str(),"%f",&beta) != 1)
    {
        printf("Invalid specification of beta\n");
        Syntax();
        return 1;
    }
    if(sscanf(params[4].c_str(),"%u",&frequency) != 1)
    {
        printf("Invalid specification of the frequency\n");
        Syntax();
        return 1;
    }

    printf("First column (0-based) of agm: %d. beta: %lf. frequency: %u Hz\n",channel,beta,frequency);

    // Load data
    printf("Loading %s...\n",params[0].c_str());
    bool *outnan;
    double *outdata;
    unsigned sx,sy;
    std::string errmsg;
    rv = matread(params[0].c_str(),outnan,outdata,sx,sy,errmsg);
    if(rv)
    {
        printf("Can't load file (%d): %s\n",rv,errmsg.c_str());
        return 1;
    }
    printf("Loaded %d x %d matrix\n",sy,sx);

    // Check that there is the right amount of data
    if(channel+9>sx)
    {
        printf("Number of columns on data file (%d) does not match the specifications of the start A-G-M column (%d)\n",sx,channel);
        return 1;
    }

    // Check if data makes sense
    /*for(unsigned i=0;i<20;i++)
    {
        printf("%u: %f\n",i,outdata[i]);
    }*/


    // Initialise gtorps
    mpu_init_gtorps(MPU_GYR_SCALE_2000);

    // Initialise Madgwick
    if(mds)
        mpu_madgwick_init_magdownsample((float)frequency,beta);
    else
        mpu_madgwick_init_magnodownsample((float)frequency,beta);

    MPUMOTIONDATA mpudata;
    MPUMOTIONGEOMETRY mpugeometry;

    FILE *fout;
    fout = fopen(params[1].c_str(),"wt");
    if(fout==0)
    {
        printf("Cannot open output file %s\n",params[1].c_str());
        return 1;
    }

    for(unsigned y=0;y<sy;y++)
    //for(unsigned y=0;y<50;y++)
    {
        // Convert data to MPUMOTIONDATA format



        mpudata.ax = outdata[y*sx + channel + 0];
        mpudata.ay = outdata[y*sx + channel + 1];
        mpudata.az = outdata[y*sx + channel + 2];
        mpudata.gx = outdata[y*sx + channel + 3];
        mpudata.gy = outdata[y*sx + channel + 4];
        mpudata.gz = outdata[y*sx + channel + 5];
        mpudata.mx = outdata[y*sx + channel + 6];
        mpudata.my = outdata[y*sx + channel + 7];
        mpudata.mz = outdata[y*sx + channel + 8];

        printf("%04u: %05d %05d %05d   %05d %05d %05d   %05d %05d %05d",y,mpudata.ax,mpudata.ay,mpudata.az,mpudata.gx,mpudata.gy,mpudata.gz,mpudata.mx,mpudata.my,mpudata.mz);
        printf("   ->   ");

        // Compute quaternion

        mpu_compute_geometry(mpudata,mpugeometry);

        printf("%f %f %f %f",mpugeometry.q0,mpugeometry.q1,mpugeometry.q2,mpugeometry.q3);


        printf("\n");

        // Write to file
        fprintf(fout,"%04u %05d %05d %05d   %05d %05d %05d   %05d %05d %05d",y,mpudata.ax,mpudata.ay,mpudata.az,mpudata.gx,mpudata.gy,mpudata.gz,mpudata.mx,mpudata.my,mpudata.mz);
        fprintf(fout,"      ");

        fprintf(fout,"%f %f %f %f",mpugeometry.q0,mpugeometry.q1,mpugeometry.q2,mpugeometry.q3);

        fprintf(fout,"\n");



    }
    fclose(fout);


    return 0;
}
