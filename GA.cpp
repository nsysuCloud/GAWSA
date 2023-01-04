#include "GA.h"
// #include "lib.h"
#include <iostream>
#include <algorithm>

using namespace myGA;
// using namespace mylib;
using namespace std;

GA::GA(int evaluation, int runtime, char cost_or_time, int n_chrom, double cross_rate, double mut_rate, d1d mCrent, d2d mCtrans, d2d mTtrans, d2d mTDataSize, d3d mTexe, int _num_machine, i1d _num_task)
{
    cost_time = cost_or_time;

    num_evaluation = evaluation;
    num_run = runtime;
    num_chromosome = n_chrom;
    crossover_rate = cross_rate;
    mutation_rate = mut_rate;

    Crent = mCrent;
    Ctrans = mCtrans;
    Ttrans = mTtrans;
    TDataSize = mTDataSize;
    Texe = mTexe;
    num_machine = _num_machine;
    num_task = _num_task;

    evaluation_score = d1d(num_evaluation, 0);
    run_best_score = d1d(num_run, 0);

    printf("[run] = %d\n", num_run);
    printf("[evaluation] = %d\n", num_evaluation);
    printf("[num_chromosome] = %d\n", num_chromosome);
    printf("[crossover_rate] = %f\n", crossover_rate);
    printf("[mutation_rate] = %f\n", mutation_rate);
    // __best_obj_vec.resize(__iter);
    overall_best = 100000.0;
    // eval_best.resize(num_evaluation);

    for (now_run = 0; now_run < num_run; now_run++)
    {
        run();
        run_best_score[now_run] = Best_score;
    }
}

void GA::run()
{
    // 1. initialize
    srand(time(NULL));
    clock_t start1 = clock();
    initialize();
    clock_t end1 = clock();
    time1 += (double)(end1 - start1) / CLOCKS_PER_SEC;
    while (now_evaluation < num_evaluation)
    {
        // 2. transition
        clock_t start2 = clock();
        chromosome = selection(chromosome, chromosome_score);
        clock_t end2 = clock();
        time2 += (double)(end2 - start2) / CLOCKS_PER_SEC;

        clock_t start3 = clock();
        chromosome = crossover(chromosome, crossover_rate);
        clock_t end3 = clock();
        time3 += (double)(end3 - start3) / CLOCKS_PER_SEC;

        clock_t start4 = clock();
        chromosome = mutation(chromosome, mutation_rate);
        clock_t end4 = clock();
        time4 += (double)(end4 - start4) / CLOCKS_PER_SEC;

        clock_t start5 = clock();
        for (int i = 0; i < num_chromosome; i++)
        {
            // 3. evaluation
            if (cost_time == 't')
                chromosome_score[i] = evaluation(chromosome[i]);
            else if (cost_time == 'c')
                chromosome_score[i] = evaluation_cost(chromosome[i]);
            // 4. determination
            determination(chromosome_score[i], chromosome[i]);
        }
        clock_t end5 = clock();
        time5 += (double)(end5 - start5) / CLOCKS_PER_SEC;

        // similarity = calculate_similarity(chromosome);
        // if (similarity > 0.8)
        // {
        //     for (int i = 0; i < chromosome.size(); i++)
        //     {
        //         if (i < chromosome.size())
        //             for (int j = 0; j < 10; j++)
        //                 chromosome[i] = transform(chromosome[i]); // HC
        //         else
        //             chromosome[i] = create_ans(Crent, Ctrans, TDataSize, Texe);

        //         i1d tmp_sol = chromosome[i];
        //         double tmp_score;
        //         if (cost_time == 't')
        //             tmp_score = evaluation(tmp_sol);
        //         else if (cost_time == 'c')
        //             tmp_score = evaluation_cost(tmp_sol);

        //         if (tmp_score < Best_score)
        //         {
        //             best_sol = tmp_sol;
        //             Best_score = tmp_score;
        //         }
        //     }
        //     // cout << "相似度：" << similarity << endl;
        // }

        // 2opt(?
        // clock_t start6 = clock();
        // int tmp = rand() % best_sol.size();
        // for (int j = 0; j < 200; j++)
        // {
        //     // i1d tmp_sol = two_opt(best_sol, tmp, tmp + j); // 2opt

        //     double tmp_score;
        //     if (cost_time == 't')
        //         tmp_score = evaluation(tmp_sol);
        //     else if (cost_time == 'c')
        //         tmp_score = evaluation_cost(tmp_sol);

        //     if (tmp_score < Best_score)
        //     {
        //         best_sol = tmp_sol;
        //         Best_score = tmp_score;
        //     }
        // }
        // clock_t end6 = clock();
        // time6 += (double)(end6 - start6) / CLOCKS_PER_SEC;

        clock_t start7 = clock();
        chromosome[0] = best_sol;
        chromosome_score[0] = Best_score;
        if (now_evaluation < num_evaluation)
        {
            for (int i = now_record; i <= now_evaluation; i++)
            {
                evaluation_score[i] += Best_score;
            }
            now_record = now_evaluation + 1;
        }
        else
        {
            for (int i = now_record; i <= num_evaluation; i++)
            {
                evaluation_score[i] += Best_score;
            }
        }

        if (Best_score < overall_best)
            overall_best = Best_score;

        // if (count_evaluation >= num_evaluation)
        // { // 結束條件
        //     now_iter = num_iter + 1;
        // }
        clock_t end7 = clock();
        time7 += (double)(end7 - start7) / CLOCKS_PER_SEC;
    }
    // cout << "time = " << t_used << ", score = " << best_score << endl;
    // cout << endl;
}

void GA::reset_data()
{
    // clear
    resource.clear();
    all_task.clear();
    resource.resize(num_machine); //設定資源
    for (int i = 0; i < resource.size(); i++)
        resource[i].execute_time = 0;
    all_task.resize(TDataSize.size());
    for (int i = 0; i < TDataSize.size(); i++)
    {
        all_task[i].resize(TDataSize[i].size());
        for (int j = 0; j < all_task[i].size(); j++)
        {
            all_task[i][j].i = i;
            all_task[i][j].j = j;
            all_task[i][j].start = 0;
            all_task[i][j].end = 0;
        }
    }
}

void GA::initialize()
{
    now_evaluation = 0;
    now_record = 0;
    reset_data();
    chromosome.resize(num_chromosome);
    chromosome_score.resize(num_chromosome);
    chromosome[0] = create_ans(Crent, Ctrans, TDataSize, Texe); //產生一組解
    if (cost_time == 't')
    {
        chromosome_score[0] = evaluation(chromosome[0]);
    }
    else if (cost_time == 'c')
    {
        chromosome_score[0] = evaluation_cost(chromosome[0]);
    }
    best_sol = chromosome[0];
    Best_score = chromosome_score[0];

    for (int i = 1; i < num_chromosome; i++)
    {
        chromosome[i] = create_ans(Crent, Ctrans, TDataSize, Texe);
        if (cost_time == 't')
            chromosome_score[i] = evaluation(chromosome[i]);
        else if (cost_time == 'c')
            chromosome_score[i] = evaluation_cost(chromosome[i]);
        determination(chromosome_score[i], chromosome[i]);
    }

    count_evaluation = 0;
}

i1d GA::create_ans(d1d Crent, d2d Ctrans, d2d TDataSize, d3d Texe)
{
    i1d tmp;
    int job_number = 0;
    for (int i = 0; i < num_task.size(); i++)
    {
        for (int j = 0; j < num_task[i]; j++)
        {
            tmp.push_back(job_number);
            job_number++;
        }
    }
    random_shuffle(tmp.begin(), tmp.end());
    return tmp;
}

double GA::evaluation(i1d sol)
{
    now_evaluation++;
    double score = 0;

    vector<int> count;
    count = vector<int>(num_task.size(), 0);
    //建立二維表
    for (int i = 0; i < sol.size(); i++)
    {
        int curr = sol[i] + 1;
        // cout << curr << endl;
        // cout << "num " << i << endl;
        int bj = 0;
        while (curr > 0)
        {
            curr = curr - num_task[bj];
            bj++;
        }
        bj--;
        int ii = bj;
        int jj = count[bj];
        all_task[ii][jj].num_resource = i % num_machine;
        resource[i % num_machine].WorkSet.push_back(all_task[ii][jj]);

        count[bj]++;
    }
    for (int i = 0; i < sol.size() / num_machine; i++)
    {
        for (int j = 0; j < num_machine; j++)
        {
            // 安排任務
            Resource resource_tmp = resource[j];
            Task task_tmp = resource_tmp.WorkSet[i];

            int num_resource = j; //給第x台機器執行

            if (task_tmp.j == 0) //任務的開頭
                task_tmp.start = resource_tmp.execute_time;

            else if (num_resource != all_task[task_tmp.i][task_tmp.j - 1].num_resource && //伺服器不同 且 上個結束時間>現在時間
                     all_task[task_tmp.i][task_tmp.j - 1].end > resource_tmp.execute_time)
            {
                task_tmp.start = all_task[task_tmp.i][task_tmp.j - 1].end;
            }
            else
                task_tmp.start = resource_tmp.execute_time;

            task_tmp.end = task_tmp.start + Texe[task_tmp.i][num_resource][task_tmp.j];

            resource_tmp.execute_time = task_tmp.end;

            all_task[task_tmp.i][task_tmp.j] = task_tmp;
            resource_tmp.WorkSet[i] = task_tmp;
            resource[j] = resource_tmp;
        }
        for (int j = 0; j < num_machine; j++)
        {
            // 計算延遲
            Resource resource_tmp = resource[j];
            Task task_tmp = resource_tmp.WorkSet[i];

            int num_resource = j; //給第x台機器執行

            if (i <= resource_tmp.WorkSet.size() - 2) //有下個任務
            {
                Task task_next = resource_tmp.WorkSet[i + 1]; //下個任務

                if (task_next.j == 0) //下個任務是開頭
                    break;
                int last_num = all_task[task_next.i][task_next.j - 1].num_resource; //上一台機器編號
                if (last_num == num_resource)                                       //同一台
                    break;
                Resource resource_last = resource[last_num];                                                   //下個任務的上個伺服器
                double Ttrans_time = TDataSize[task_next.i][task_next.j - 1] / Ttrans[num_resource][last_num]; //時間間隔

                if (resource_last.execute_time > resource_tmp.execute_time)
                {
                    resource_last.execute_time += Ttrans_time;
                    resource_tmp.execute_time = resource_last.execute_time;
                }
                else if (resource_last.execute_time < resource_tmp.execute_time)
                {
                    resource_tmp.execute_time += Ttrans_time;
                    resource_last.execute_time = resource_tmp.execute_time;
                }

                resource[j] = resource_tmp;
                resource[all_task[task_next.i][task_next.j - 1].num_resource] = resource_last; //下個任務的上個伺服器
            }
        }
    }
    // //cout 資源的工作集
    // for (int i = 0; i < resource.size(); i++)
    // {
    //     for (int j = 0; j < resource[i].WorkSet.size(); j++)
    //         cout << "i = " << resource[i].WorkSet[j].i << ", "
    //              << "j = " << resource[i].WorkSet[j].j << endl;
    //     cout << "--------" << endl;
    // }
    // //cout 排程結果
    // for (int i = 0; i < resource.size(); i++)
    // {
    //     for (int j = 0; j < resource[i].WorkSet.size(); j++)
    //     {
    //         Task task_tmp = resource[i].WorkSet[j];
    //         cout << "i = " << task_tmp.i << ", j = " << task_tmp.j << ", start = " << task_tmp.start << ", end = " << task_tmp.end << endl;
    //     }
    //     cout << "--------" << endl;
    // }
    for (int i = 0; i < resource.size(); i++)
        if (resource[i].execute_time > score)
            score = resource[i].execute_time;
    reset_data();

    // if (count_evaluation % 1000 == 0)
    //     cout << count_evaluation << "," << best_score << endl;
    // cout << "~~~";
    // if (count_evaluation <= num_evaluation)
    //     eval_best[count_evaluation] += best_score;

    count_evaluation++; // 0822
    // cout << "printScore: " << score << endl;
    return score;
}

double GA::evaluation_cost(i1d sol)
{
    now_evaluation++;
    double score = 0, cost = 0;

    vector<int> count;
    count = vector<int>(num_task.size(), 0);
    //建立二維表
    for (int i = 0; i < sol.size(); i++)
    {
        int curr = sol[i] + 1;
        // cout << curr << endl;
        // cout << "num " << i << endl;
        int bj = 0;
        while (curr > 0)
        {
            curr = curr - num_task[bj];
            bj++;
        }
        bj--;
        int ii = bj;
        int jj = count[bj];
        all_task[ii][jj].num_resource = i % num_machine;
        resource[i % num_machine].WorkSet.push_back(all_task[ii][jj]);

        count[bj]++;
    }

    for (int i = 0; i < sol.size() / num_machine; i++)
    {
        for (int j = 0; j < num_machine; j++)
        {
            // 安排任務
            Resource resource_tmp = resource[j];
            Task task_tmp = resource_tmp.WorkSet[i];

            int num_resource = j; //給第x台機器執行

            if (task_tmp.j == 0) //任務的開頭
                task_tmp.start = resource_tmp.execute_time;

            else if (num_resource != all_task[task_tmp.i][task_tmp.j - 1].num_resource && //伺服器不同 且 上個結束時間>現在時間
                     all_task[task_tmp.i][task_tmp.j - 1].end > resource_tmp.execute_time)
            {
                task_tmp.start = all_task[task_tmp.i][task_tmp.j - 1].end;
                if (num_resource != all_task[task_tmp.i][task_tmp.j - 1].num_resource)
                    cost += TDataSize[task_tmp.i][task_tmp.j] * Ctrans[num_resource][all_task[task_tmp.i][task_tmp.j - 1].num_resource]; // 加上傳送時間
            }
            else
                task_tmp.start = resource_tmp.execute_time;

            task_tmp.end = task_tmp.start + Texe[task_tmp.i][num_resource][task_tmp.j];

            resource_tmp.execute_time = task_tmp.end;

            all_task[task_tmp.i][task_tmp.j] = task_tmp;
            resource_tmp.WorkSet[i] = task_tmp;
            resource[j] = resource_tmp;
        }
        for (int j = 0; j < num_machine; j++)
        {
            // 計算延遲
            Resource resource_tmp = resource[j];
            Task task_tmp = resource_tmp.WorkSet[i];

            int num_resource = j; //給第x台機器執行

            if (i <= resource_tmp.WorkSet.size() - 2) //有下個任務
            {
                Task task_next = resource_tmp.WorkSet[i + 1]; //下個任務

                if (task_next.j == 0) //下個任務是開頭
                    break;
                int last_num = all_task[task_next.i][task_next.j - 1].num_resource; //上一台機器編號
                if (last_num == num_resource)                                       //同一台
                    break;
                Resource resource_last = resource[last_num];                                                   //下個任務的上個伺服器
                double Ttrans_time = TDataSize[task_next.i][task_next.j - 1] / Ttrans[num_resource][last_num]; //時間間隔

                if (resource_last.execute_time > resource_tmp.execute_time)
                {
                    resource_last.execute_time += Ttrans_time;
                    resource_tmp.execute_time = resource_last.execute_time;
                }
                else if (resource_last.execute_time < resource_tmp.execute_time)
                {
                    resource_tmp.execute_time += Ttrans_time;
                    resource_last.execute_time = resource_tmp.execute_time;
                }

                resource[j] = resource_tmp;
                resource[all_task[task_next.i][task_next.j - 1].num_resource] = resource_last; //下個任務的上個伺服器
            }
        }
    }
    // //cout 資源的工作集
    // for (int i = 0; i < resource.size(); i++)
    // {
    //     for (int j = 0; j < resource[i].WorkSet.size(); j++)
    //         cout << "i = " << resource[i].WorkSet[j].i << ", "
    //              << "j = " << resource[i].WorkSet[j].j << endl;
    //     cout << "--------" << endl;
    // }
    // //cout 排程結果
    // for (int i = 0; i < resource.size(); i++)
    // {
    //     for (int j = 0; j < resource[i].WorkSet.size(); j++)
    //     {
    //         Task task_tmp = resource[i].WorkSet[j];
    //         cout << "i = " << task_tmp.i << ", j = " << task_tmp.j << ", start = " << task_tmp.start << ", end = " << task_tmp.end << endl;
    //     }
    //     cout << "--------" << endl;
    // }
    for (int i = 0; i < resource.size(); i++)
        if (resource[i].execute_time > score)
            score = resource[i].execute_time;
    for (int i = 0; i < resource.size(); i++)
        cost += resource[i].execute_time * Crent[i];
    if (score > 400)
        cost += score * 10;
    reset_data();

    // if (count_evaluation % 1000 == 0)
    //     cout << count_evaluation << "," << best_score << endl;
    // if (count_evaluation <= num_evaluation)
    //     eval_best[count_evaluation] += best_score;

    count_evaluation++; // 0822

    // cout << "printCost: " << cost << endl;
    return cost;
}

i2d GA::selection(i2d chromosome, d1d chromosome_score)
{

    // fitness
    double tmp = 0;
    vector<double> scale;

    for (int i = 0; i < chromosome.size(); i++)
        tmp += (double)1 / chromosome_score[i];
    for (int i = 0; i < chromosome.size(); i++)
        scale.push_back(((double)1 / chromosome_score[i]) / tmp);
    for (int i = 1; i < chromosome.size(); i++)
        scale[i] += scale[i - 1];

    // selection
    double rand_num;
    i2d new_chromosome;
    new_chromosome.clear();

    for (int i = 0; i < chromosome.size(); i++)
    {
        rand_num = static_cast<double>(rand()) / RAND_MAX;

        if (rand_num <= scale[0])
            new_chromosome.push_back(chromosome[0]);
        else if (rand_num == 1)
            new_chromosome.push_back(chromosome[chromosome.size() - 1]);
        else
            for (int j = 0; j < scale.size(); j++)
            {
                if (rand_num >= scale[j - 1] && rand_num < scale[j])
                {
                    new_chromosome.push_back(chromosome[j]);
                    break;
                }
            }
    }
    //	new_chromosome[0] = best_sol;
    return new_chromosome;
}

i2d GA::crossover(i2d chromosome, double rate)
{
    i2d new_chromosome;
    i1d tmp_sol1, tmp_sol2;
    int num1, num2;
    double rand_num;
    for (int i = 0; i < num_chromosome / 2; i++)
    {
        //隨機取兩條染色體
        num1 = rand() % chromosome.size();
        tmp_sol1 = chromosome[num1];
        num2 = rand() % chromosome.size();
        tmp_sol2 = chromosome[num2];

        //是否交換染色體
        rand_num = static_cast<double>(rand()) / RAND_MAX;
        if (rand_num < rate)
        {
            vector<bool> mask = vector<bool>(tmp_sol1.size(), 0);
            int mask_count = 0;
            while (mask_count < tmp_sol1.size() / 3)
            {
                int start_pos = rand() % tmp_sol1.size();
                while (mask[start_pos] == 1)
                {
                    start_pos = rand() % tmp_sol1.size();
                }
                int start_value = tmp_sol1[start_pos];
                int x = start_pos;
                mask[x] = 1;
                mask_count++;
                do
                {
                    for (int co = 0; co < tmp_sol1.size(); co++)
                    {
                        if (tmp_sol1[co] == tmp_sol2[x])
                        {
                            x = co;
                            mask[x] = 1;
                            mask_count++;
                            break;
                        }
                    }
                } while (start_value != tmp_sol2[x]);
            }
            for (int l = 0; l < tmp_sol1.size(); l++)
            {
                if (mask[l] == 0)
                {
                    chromosome[num1][l] = tmp_sol2[l];
                    chromosome[num2][l] = tmp_sol1[l];
                }
                else
                {
                    chromosome[num1][l] = tmp_sol1[l];
                    chromosome[num2][l] = tmp_sol2[l];
                }
            }
        }
    }
    return chromosome;
}

i1d GA::find_order(i1d half_sol, i1d sol)
{
    i1d new_half_sol;

    for (int i = 0; i < sol.size(); i++)
        for (int j = 0; j < half_sol.size(); j++)
            if (sol[i] == half_sol[j])
            {
                new_half_sol.push_back(sol[i]);
                half_sol.erase(half_sol.begin() + j);
                j = half_sol.size();
            }

    return new_half_sol;
}

i2d GA::mutation(i2d chromosome, double rate)
{
    double rand_num;
    int tmp;

    for (int i = 0; i < chromosome.size(); i++)
    {
        int tmp1, tmp2;
        tmp1 = rand() % chromosome[i].size();
        tmp2 = rand() % chromosome[i].size();
        swap(chromosome[i][tmp1], chromosome[i][tmp2]);
    }
    return chromosome;
}

i1d GA::transform(i1d now_sol)
{
    int num1, num2, tmp;
    do
    {
        num1 = rand() % now_sol.size();
        num2 = rand() % now_sol.size();
    } while (num1 == num2);

    tmp = now_sol[num1];
    now_sol[num1] = now_sol[num2];
    now_sol[num2] = tmp;
    return now_sol;
}

bool GA::determination(double now_score, i1d sol)
{
    if (Best_score > now_score)
    {
        Best_score = now_score;
        best_sol = sol;
        return true;
    }
    return false;
}

// double GA::calculate_similarity(i2d chromosome)
// {
//     int same = 0;
//     for (int i = 2; i < chromosome.size(); i++)
//         if (chromosome[i] == chromosome[1])
//             same++;

//     return (double)same / (chromosome.size() - 2);
// }

i1d GA::two_opt(i1d sol, int point1, int point2)
{
    i1d new_sol = sol;
    int tmp;

    if (point2 >= sol.size())
        point2 -= sol.size();

    // if (point1 > point2)
    // {
    //     tmp = point1;
    //     point1 = point2;
    //     point2 = tmp;
    // }

    // tmp = point2 - point1;
    // for (int i = 0; i <= tmp; i++)
    //     new_sol[point1 + i] = sol[point2 - i];

    new_sol[point1] = sol[point2];
    new_sol[point2] = sol[point1];
    return new_sol;
}

void GA::print_sol(i2d chromosome, int index)
{
    for (int i = 0; i < chromosome[index].size(); i++)
        printf("[chromosome[%d]]: %d\n", i, chromosome[index][i]);
}

void GA::print_score(d1d chromosome_score)
{
    for (int i = 0; i < chromosome_score.size(); i++)
        printf("[score[%d]]: %f\n", i, chromosome_score[i]);
}