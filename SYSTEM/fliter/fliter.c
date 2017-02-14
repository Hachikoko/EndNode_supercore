#include "fliter.h"
#include "link_queue.h"
#include "main.h"
#include "string.h"


//中位值平均滤波算法


void fill_data(Splited_Raw_Data* ptr_split_raw_data,Link_Queue* prt_link_raw_data){
	
	Queue_Node* ptr_node = prt_link_raw_data->front->next;
	
	for(int i = 0; i < FLITER_N;i++){
		ptr_split_raw_data->ax[i] = ptr_node->data.ax;
		ptr_split_raw_data->ay[i] = ptr_node->data.ay;
		ptr_split_raw_data->az[i] = ptr_node->data.az;
		ptr_split_raw_data->gx[i] = ptr_node->data.gx;
		ptr_split_raw_data->gy[i] = ptr_node->data.gy;
		ptr_split_raw_data->gz[i] = ptr_node->data.gz;
		ptr_split_raw_data->mx[i] = ptr_node->data.mx;
		ptr_split_raw_data->my[i] = ptr_node->data.my;
		ptr_split_raw_data->mz[i] = ptr_node->data.mz;
		
		ptr_node = ptr_node->next;
	}
	return;
}

void sort_split_data(Splited_Raw_Data* ptr_split_raw_data){
	short temp;
	for(int i = 0; i < (FLITER_N - 1);i++){
		for(int j = 0; j < (FLITER_N - 1 - i); i++){
			if(ptr_split_raw_data->ax[j] > ptr_split_raw_data->ax[j+1]){
				temp = ptr_split_raw_data->ax[j];
				ptr_split_raw_data->ax[j] = ptr_split_raw_data->ax[j+1];
				ptr_split_raw_data->ax[j+1] = temp;
			}
			if(ptr_split_raw_data->ay[j] > ptr_split_raw_data->ay[j+1]){
				temp = ptr_split_raw_data->ay[j];
				ptr_split_raw_data->ay[j] = ptr_split_raw_data->ay[j+1];
				ptr_split_raw_data->ay[j+1] = temp;
			}
			if(ptr_split_raw_data->az[j] > ptr_split_raw_data->az[j+1]){
				temp = ptr_split_raw_data->az[j];
				ptr_split_raw_data->az[j] = ptr_split_raw_data->az[j+1];
				ptr_split_raw_data->az[j+1] = temp;
			}
			if(ptr_split_raw_data->gx[j] > ptr_split_raw_data->gx[j+1]){
				temp = ptr_split_raw_data->gx[j];
				ptr_split_raw_data->gx[j] = ptr_split_raw_data->gx[j+1];
				ptr_split_raw_data->gx[j+1] = temp;
			}
			if(ptr_split_raw_data->gy[j] > ptr_split_raw_data->gy[j+1]){
				temp = ptr_split_raw_data->gy[j];
				ptr_split_raw_data->gy[j] = ptr_split_raw_data->gy[j+1];
				ptr_split_raw_data->gy[j+1] = temp;
			}
			if(ptr_split_raw_data->gz[j] > ptr_split_raw_data->gz[j+1]){
				temp = ptr_split_raw_data->gz[j];
				ptr_split_raw_data->gz[j] = ptr_split_raw_data->gz[j+1];
				ptr_split_raw_data->gz[j+1] = temp;
			}
			if(ptr_split_raw_data->mx[j] > ptr_split_raw_data->mx[j+1]){
				temp = ptr_split_raw_data->mx[j];
				ptr_split_raw_data->mx[j] = ptr_split_raw_data->mx[j+1];
				ptr_split_raw_data->mx[j+1] = temp;
			}
			if(ptr_split_raw_data->my[j] > ptr_split_raw_data->my[j+1]){
				temp = ptr_split_raw_data->my[j];
				ptr_split_raw_data->my[j] = ptr_split_raw_data->my[j+1];
				ptr_split_raw_data->my[j+1] = temp;
			}
			if(ptr_split_raw_data->mz[j] > ptr_split_raw_data->mz[j+1]){
				temp = ptr_split_raw_data->mz[j];
				ptr_split_raw_data->mz[j] = ptr_split_raw_data->mz[j+1];
				ptr_split_raw_data->mz[j+1] = temp;
			}
			
		}
	}
	
	return;
}

Fliter_Result* middle_average_filter(Link_Queue* prt_link_raw_data,Fliter_Result* ptr_fliter_result){
	Splited_Raw_Data split_raw_data;
	memset(ptr_fliter_result,0,sizeof(Fliter_Result));
	fill_data(&split_raw_data,prt_link_raw_data);
	sort_split_data(&split_raw_data);
	for(int i = 1; i < (FLITER_N -1);i++){
		ptr_fliter_result->ax += split_raw_data.ax[i];
		ptr_fliter_result->ay += split_raw_data.ay[i];
		ptr_fliter_result->az += split_raw_data.az[i];
		ptr_fliter_result->gx += split_raw_data.gx[i];
		ptr_fliter_result->gy += split_raw_data.gy[i];
		ptr_fliter_result->gz += split_raw_data.gz[i];
		ptr_fliter_result->mx += split_raw_data.mx[i];
		ptr_fliter_result->my += split_raw_data.my[i];
		ptr_fliter_result->mz += split_raw_data.mz[i];
		
	}
	
	ptr_fliter_result->ax = (ptr_fliter_result->ax)/(FLITER_N - 2);
	ptr_fliter_result->ay = (ptr_fliter_result->ay)/(FLITER_N - 2);
	ptr_fliter_result->az = (ptr_fliter_result->az)/(FLITER_N - 2);
	ptr_fliter_result->gx = (ptr_fliter_result->gx)/(FLITER_N - 2);
	ptr_fliter_result->gy = (ptr_fliter_result->gy)/(FLITER_N - 2);
	ptr_fliter_result->gz = (ptr_fliter_result->gz)/(FLITER_N - 2);
	ptr_fliter_result->mx = (ptr_fliter_result->mx)/(FLITER_N - 2);
	ptr_fliter_result->my = (ptr_fliter_result->my)/(FLITER_N - 2);
	ptr_fliter_result->mz = (ptr_fliter_result->mz)/(FLITER_N - 2);
	
	return ptr_fliter_result;
}

