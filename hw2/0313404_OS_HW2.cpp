#include<iostream>
#include<fstream>
#include<vector>
#include<queue>
#include<algorithm>

using namespace std;

struct process {
	int id = -1, arr_t = -1, cpu_t1 = -1, io_t = -1, cpu_t2 = -1, rr_num = 0;
	bool b1_done = false;
	bool operator < (const process& str) const {
		return (id < str.id);
	}
};

void SJF(vector<int> data, int num) {
	ofstream fout("0313404.txt");
	process p1, p2, p3, p4, p5, p6, p7, p8;
	vector<process> process_pool = {p1, p2, p3, p4, p5, p6, p7, p8};
	/* set process attribute */
	for (int i = 0; i < num; i++) {
		process_pool.at(i).id = data.at(5 * i + 2);
		process_pool.at(i).arr_t = data.at(5 * i + 3);
		process_pool.at(i).cpu_t1 = data.at(5 * i + 4);
		process_pool.at(i).io_t = data.at(5 * i + 5);
		process_pool.at(i).cpu_t2 = data.at(5 * i + 6);
	}
	/* erase unused process */
	for (int i = 1; i <= 8-num; i++)
		process_pool.erase(process_pool.begin() + num);

	int timer = 0, cpu_free_time = 0;

	while (process_pool.size() != 0) {
		if (timer == cpu_free_time) {
			int index = -1;
			for (int i = 0; i < process_pool.size(); i++) {
				/* i's arrival time <= timer */
				if (process_pool.at(i).arr_t <= timer) {
					if (index == -1) 
						index = i;
					/* i's burst time < index's burst time */
					if (process_pool.at(i).cpu_t1 < process_pool.at(index).cpu_t1) 
						index = i;					
					/* i's burst time = index's burst time */
					else if (process_pool.at(i).cpu_t1 == process_pool.at(index).cpu_t1) {
						/* i's arrival time < index's arrival time */
						if (process_pool.at(i).arr_t < process_pool.at(index).arr_t) 
							index = i;
						/* i's arrival time = index's arrival time */
						else if (process_pool.at(i).arr_t == process_pool.at(index).arr_t) {
							/* i's id < index's id */
							if (process_pool.at(i).id < process_pool.at(index).id) 
								index = i;
						}
					}
				}
			}
			if (index == -1) {
				timer++;
				cpu_free_time++;
				continue;
			}
			if (!process_pool.at(index).b1_done) {
				process_pool.at(index).arr_t = timer + process_pool.at(index).cpu_t1 + process_pool.at(index).io_t;				
				cpu_free_time = timer + process_pool.at(index).cpu_t1;
				process_pool.at(index).cpu_t1 = process_pool.at(index).cpu_t2;
				process_pool.at(index).b1_done = true;
			}
			else if (process_pool.at(index).b1_done) {
				cpu_free_time = timer + process_pool.at(index).cpu_t1;
				fout << process_pool.at(index).id << ":" << cpu_free_time;
				process_pool.erase(process_pool.begin() + index);
				if (process_pool.size() != 0)
					fout << endl;
			}
		}
		else 
			timer++;		
	}
	fout.close();
}

void SRTF(vector<int> data, int num) {
	ofstream fout("0313404.txt");
	process p1, p2, p3, p4, p5, p6, p7, p8;
	vector<process> process_pool = { p1, p2, p3, p4, p5, p6, p7, p8 };
	/* set process attribute */
	for (int i = 0; i < num; i++) {
		process_pool.at(i).id = data.at(5 * i + 2);
		process_pool.at(i).arr_t = data.at(5 * i + 3);
		process_pool.at(i).cpu_t1 = data.at(5 * i + 4);
		process_pool.at(i).io_t = data.at(5 * i + 5);
		process_pool.at(i).cpu_t2 = data.at(5 * i + 6);
	}
	/* erase unused process */
	for (int i = 1; i <= 8 - num; i++)
		process_pool.erase(process_pool.begin() + num);

	int timer = 0;

	while (process_pool.size() != 0) {
		int index = -1;
		for (int i = 0; i < process_pool.size(); i++) {
			/* i's arrival time <= timer */
			if (process_pool.at(i).arr_t <= timer) {
				if (index == -1)
					index = i;
				/* i's burst time < index's burst time */
				if (process_pool.at(i).cpu_t1 < process_pool.at(index).cpu_t1)
					index = i;
				/* i's burst time = index's burst time */
				else if (process_pool.at(i).cpu_t1 == process_pool.at(index).cpu_t1) {
					/* i's arrival time < index's arrival time */
					if (process_pool.at(i).arr_t < process_pool.at(index).arr_t)
						index = i;
					/* i's arrival time = index's arrival time */
					else if (process_pool.at(i).arr_t == process_pool.at(index).arr_t) {
						/* i's id < index's id */
						if (process_pool.at(i).id < process_pool.at(index).id)
							index = i;
					}
				}
			}
		}
		if (index == -1) {
			timer++;
			//cout << timer << " = p" << process_pool.at(index).id << endl;
			continue;
		}
		if (!process_pool.at(index).b1_done) {
			process_pool.at(index).cpu_t1--;
			timer++;
			//cout << timer << " = p" << process_pool.at(index).id << endl;
			if (process_pool.at(index).cpu_t1 == 0) {
				process_pool.at(index).b1_done = true;
				process_pool.at(index).cpu_t1 = process_pool.at(index).cpu_t2;
				process_pool.at(index).arr_t = timer + process_pool.at(index).io_t;
			}
		}
		else if (process_pool.at(index).b1_done) {
			process_pool.at(index).cpu_t1--;
			timer++;
			//cout << timer << " = p" << process_pool.at(index).id << endl;
			if (process_pool.at(index).cpu_t1 == 0) {
				fout << process_pool.at(index).id << ":" << timer;
				process_pool.erase(process_pool.begin() + index);
				if (process_pool.size() != 0)
					fout << endl;
			}
		}
	}
	fout.close();
}

void MLFQ(vector<int> data, int num) {
	ofstream fout("0313404.txt");
	process p1, p2, p3, p4, p5, p6, p7, p8;
	vector<process> process_pool = { p1, p2, p3, p4, p5, p6, p7, p8 };
	vector<process> temp;
	/* set process attribute */
	for (int i = 0; i < num; i++) {
		process_pool.at(i).id = data.at(5 * i + 2);
		process_pool.at(i).arr_t = data.at(5 * i + 3);
		process_pool.at(i).cpu_t1 = data.at(5 * i + 4);
		process_pool.at(i).io_t = data.at(5 * i + 5);
		process_pool.at(i).cpu_t2 = data.at(5 * i + 6);
	}
	int timer = 0;
	int process_cnt = 0;
	queue<process> RR_queue;
	queue<process> FCFS_queue;
	while (process_cnt != num) {
		sort(process_pool.begin(), process_pool.end());
		/* push process to RR_queue */
		for (int i = 0; i < process_pool.size(); i++) {
			if (process_pool.at(i).arr_t == timer) {
				RR_queue.push(process_pool.at(i));
				process_pool.erase(process_pool.begin() + i);
			}
		}
		sort(temp.begin(), temp.end());
		/* push process to FCFS_queue */
		for (int i = 0; i < temp.size(); i++) {
			if (temp.at(i).arr_t == timer) {
				FCFS_queue.push(temp.at(i));
				temp.erase(temp.begin() + i);
			}
		}

		if (RR_queue.size() != 0) {
			RR_queue.front().cpu_t1--;
			RR_queue.front().rr_num++;
			timer++;
			if (RR_queue.front().cpu_t1 == 0) {
				if (RR_queue.front().b1_done) {
					fout << RR_queue.front().id << ":" << timer;
					RR_queue.pop();
					process_cnt++;
					if (process_cnt != num)
						fout << endl;
				}
				else {
					RR_queue.front().cpu_t1 = RR_queue.front().cpu_t2;
					RR_queue.front().arr_t = timer + RR_queue.front().io_t;
					RR_queue.front().rr_num = 0;
					RR_queue.front().b1_done = true;
					process_pool.push_back(RR_queue.front());
					RR_queue.pop();
				}
			}
			else if (RR_queue.front().rr_num == 4) {
				RR_queue.front().arr_t = timer;
				temp.push_back(RR_queue.front());
				RR_queue.pop();
			}
		}
		else if (RR_queue.size() == 0) {
			FCFS_queue.front().cpu_t1--;
			timer++;
			if (FCFS_queue.front().cpu_t1 == 0) {
				if (FCFS_queue.front().b1_done) {
					fout << FCFS_queue.front().id << ":" << timer;
					FCFS_queue.pop();
					process_cnt++;
					if (process_cnt != num)
						fout << endl;
				}
				else {
					FCFS_queue.front().cpu_t1 = FCFS_queue.front().cpu_t2;
					FCFS_queue.front().arr_t = timer + FCFS_queue.front().io_t;
					FCFS_queue.front().b1_done = true;
					temp.push_back(FCFS_queue.front());
					FCFS_queue.pop();
				}
			}
		}
	}
	fout.close();
}

int main() {	
	/* read input.txt to vector data */
	ifstream fin;
	fin.open("input.txt");
	if (!fin) {
		cout << "open input.txt failed" << endl;
		return 0;
	}
	int temp;
	vector<int> data;
	while (fin >> temp)
		data.push_back(temp);
	fin.close();

	int q = data.at(0);	  //   q = question1 or 2 or 3
	int num = data.at(1); // num = processes number

	/* chose scheduling algorithm */
	switch (q) {
		case 1:
			SJF(data, num);
			break;
		case 2:
			SRTF(data, num);
			break;
		case 3:
			MLFQ(data, num);
			break;
		default:
			cout << "question number error!!" << endl;
			break;
	}
	return 0;
}
