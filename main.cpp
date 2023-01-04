// #define TEST true
#define TEST false

#include "GA.h"

#include <iostream>
#include <time.h>
#include <math.h>
#include <fstream>
#include <sstream>

using namespace std;

d1d Crent;                     //雲端資源租用成本
d2d Ctrans, Ttrans, TDataSize; //資源通訊成本、任務檔案大小
d3d Texe;                      //執行時間
int num_machine;               //機器數量
i1d num_task;                  //子任務數量
string inputfile;

void read_dataset();

int main(int argc, char *argv[])
{
    // i1d best_obj_round(iter, 0);
    // ofstream fout1;
    ofstream fout2;
    d1d Before_score, iter_best, eval_best;
    double Overall_best;
    double t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0;
    int two_opt_eval = 0;
    i1d end_iter, end_eval;
    int count_eval = 0;
    int plus_eval = 0;
    d1d eval_score;
    d1d run_best;

    // ./$(EXE_NAME) ${algo} ${runtime} ${iter} ${evaluation} ${inputfile} ${cost_or_time} ${population_num}
    int evaluation = stoi(argv[1]);
    int runtime = stoi(argv[2]);
    char cost_or_time = *argv[3];
    int num_chromosome = stoi(argv[4]);
    double crossover_rate = stod(argv[5]);
    double mutation_rate = stod(argv[6]);
    inputfile = argv[7];
    string resultname = argv[8];

    string result_a = "result_iter" + resultname + ".txt";
    string result_b = "result_eval" + resultname + ".txt";

    // fout1.open(result_a);
    // if (!fout1)
    //     cout << "Writing file is fail!" << endl;
    fout2.open(result_b);
    if (!fout2)
        cout << "Writing file is fail!" << endl;

    read_dataset();

    if (!TEST)
    {
        clock_t start = clock();
        myGA::GA generic(evaluation, runtime, cost_or_time, num_chromosome, crossover_rate, mutation_rate, Crent, Ctrans, Ttrans, TDataSize, Texe, num_machine, num_task);
        // Before_score = generic.before_score;
        Overall_best = generic.overall_best;
        eval_score = generic.evaluation_score;
        run_best = generic.run_best_score;
        t1 = generic.time1;
        t2 = generic.time2;
        t3 = generic.time3;
        t4 = generic.time4;
        t5 = generic.time5;
        t6 = generic.time6;
        t7 = generic.time7;

        clock_t end = clock();
        double t_used = (double)(end - start) / CLOCKS_PER_SEC;

        double avg_score = 0;
        for (int i = 0; i < run_best.size(); i++)
        {
            cout << "run " << i << " best: " << run_best[i] << endl;
            avg_score += run_best[i];
        }
        avg_score = avg_score / run_best.size();
        cout << endl;
        cout << "best value = " << Overall_best << endl;
        cout << endl;
        cout << "average    = " << avg_score << endl;
        cout << endl;
        for (int i = 0; i < eval_score.size(); i++)
        {
            fout2 << eval_score[i] / runtime << endl;
        }

        // calculate standard deviation
        if (runtime > 1)
        {
            double std = 0.0;
            for (int i = 0; i < run_best.size(); i++)
                std += pow(run_best[i] - avg_score, 2);
            std /= (run_best.size() - 1);
            std = sqrt(std);
            cout << "standard deviation: " << std << endl;
            cout << endl;
        }
        cout << "time     = " << t_used << " sec" << endl;
        cout << endl;
        cout << "time_avg = " << t_used / runtime << " sec" << endl;
        cout << endl;
        cout << "time_init                        : " << t1 << " sec (" << 100 * (t1 / t_used) << "%)" << endl;
        cout << "time_selection                   : " << t2 << " sec (" << 100 * (t2 / t_used) << "%)" << endl;
        cout << "time_crossover                   : " << t3 << " sec (" << 100 * (t3 / t_used) << "%)" << endl;
        cout << "time_mutation                    : " << t4 << " sec (" << 100 * (t4 / t_used) << "%)" << endl;
        cout << "time_evaluation                  : " << t5 << " sec (" << 100 * (t5 / t_used) << "%)" << endl;
        cout << "time_2-opt                       : " << t6 << " sec (" << 100 * (t6 / t_used) << "%)" << endl;
        cout << "time_other                       : " << t7 << " sec (" << 100 * (t7 / t_used) << "%)" << endl;
        cout << endl;
        // cout << "two_opt_eval: " << two_opt_eval << endl;
        // cout << "count_eval  : " << count_eval << endl;
        // cout << "plus_eval   : " << plus_eval << endl;
        // cout << "two_opt/count_eval : " << (float)two_opt_eval / count_eval << endl;
        int num_run = runtime;
        // for (int i = 0; i < iter_best.size(); i++)
        //     fout1 << iter_best[i] / num_run << endl;
        for (int i = 0; i <= eval_score.size(); i++)
            fout2 << eval_score[i] / num_run << endl;
        // fout1.close();
        fout2.close();
    }

    printf("done!!\n");
    return 0;
}

void read_dataset()
{
    ifstream myfile;
    myfile.open(inputfile);

    int status = 0; // 0.null 1.Crent 2.Ctrans 3.TDataSize 4.Texe

    //暫存空間
    string line;
    stringstream ssline;
    double ftmp;
    d1d vftmp;
    char *pEnd;

    while (getline(myfile, line))
    {
        if (line[line.size() - 1] == '\r')
            line.resize(line.size() - 1);

        stringstream ssline(line);

        if (line == "C rent")
            status = 1;
        else if (line == "C trans")
            status = 2;
        else if (line == "T trans")
            status = 3;
        else if (line == "T datasize")
            status = 4;
        else if (line == "exe")
            status = 5;
        else if (line == "Fin")
        {
            if (status == 4 || status == 5)
                Texe.resize(Texe.size() + 1);
            status = 0;
        }

        if (status == 1)
        {
            ftmp = strtof(line.c_str(), &pEnd);
            Crent.push_back(ftmp);
        }
        else if (status == 2 || status == 3 || status == 4 || status == 5)
        {
            vftmp.clear();
            while (getline(ssline, line, ','))
            {
                ftmp = strtof(line.c_str(), &pEnd);
                vftmp.push_back(ftmp);
            }
            if (status == 2)
                Ctrans.push_back(vftmp);
            if (status == 3)
                Ttrans.push_back(vftmp);
            if (status == 4)
                TDataSize.push_back(vftmp);
            if (status == 5)
                Texe[Texe.size() - 1].push_back(vftmp);
        }
    }

    Crent.erase(Crent.begin()); //刪除第一個
    Ctrans.erase(Ctrans.begin());
    Ttrans.erase(Ttrans.begin());
    TDataSize.erase(TDataSize.begin());
    Texe.erase(Texe.end());
    for (int k = 0; k < Texe.size(); k++)
        Texe[k].erase(Texe[k].begin());

    myfile.close();

    num_machine = Crent.size(); //機器的數量
    for (int i = 0; i < TDataSize.size(); i++)
        num_task.push_back(TDataSize[i].size()); //每個任務的子任務數量
}
