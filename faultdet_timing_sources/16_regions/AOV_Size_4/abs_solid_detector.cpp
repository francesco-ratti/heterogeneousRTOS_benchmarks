#include "parameters.h"
#include "region.h"
#include <ap_int.h>
#include "hls_stream.h"
#include <math.h>
#include <string.h> // memcpy

const float thresh=THRESH;

bool hasRegion(const region_t regions[MAX_REGIONS], const ap_uint<8> n_regions, const float d[MAX_AOV_DIM]){
	for(int i=0; i < MAX_REGIONS; i++){
		if (i>=n_regions)
			break;
//#pragma HLS UNROLL

		//		if (i>=n_regions)
		//			break;
//#pragma HLS loop_tripcount min=0 max=1
		#pragma HLS PIPELINE II=4
		for(int j=0; j < MAX_AOV_DIM; j++){
			#pragma HLS unroll
//#pragma HLS loop_tripcount min=1 max=8

			if((( regions[i].min[j] <= d[j] ) && ( regions[i].max[j] >= d[j] )) /*||
			(	( regions[i].min[j] > d[j]  && ( regions[i].min[j] - d[j] < thresh )) ||
				( d[j] > regions[i].min[j] && ( d[j] - regions[i].min[j] < thresh )) ||
				( regions[i].max[j] > d[j]  && ( regions[i].max[j] - d[j] < thresh )) ||
				( d[j] > regions[i].max[j] && ( d[j] - regions[i].max[j] < thresh ))
			)*/
			) {
				if (j==MAX_AOV_DIM-1)
					return true;
			} else break;
		}
	}
	return false;
}

bool is_valid(const float val[MAX_AOV_DIM]){

	is_valid_label0:is_valid_label2:for(int i=0; i < MAX_AOV_DIM;  i++) {
		//#pragma HLS PIPELINE II=1
		//#pragma HLS unroll
		if(isnan(val[i]) || val[i] == INFINITY || val[i] == -INFINITY)
			return false;
	}
	return true;
}

void insert_point(region_t regions[MAX_REGIONS], ap_uint<4> &n_regions, const float d[MAX_AOV_DIM]) {//, bool is_accept){

	//int id = find_region(regions, n_regions, d);

	if (is_valid(d)) { //&& id<0) {
		//create a new node.
		insert_point_label4:for(int i=0; i < MAX_AOV_DIM; i++){
			regions[n_regions].min[i] = regions[n_regions].max[i] = regions[n_regions].center[i] = d[i];
		}
//		n_regions++;

		//if we're full of space, make room for another region.
		if(n_regions == MAX_REGIONS-1){ //if we're full.
			//find the region with the most similar dynamics that isn't
			//completely obstructed by another region.
			ap_uint<4> merge_1=0;
			bool merge_1_vld=false;

			ap_uint<4> merge_2=0;
			float score = 0;

			ap_uint<4> i_real=0;
			ap_uint<4> k_real=1;

			float tmp_score=0;
			ap_uint<4> tmp_other=0;
			bool tmp_other_vld=false;

			//MAX_REGIONS_SUMM
			while (i_real < MAX_REGIONS-1){
#pragma HLS PIPELINE

				//#pragma HLS unroll
				#pragma HLS loop_tripcount min=0 max=136

				float distance = 0;
				float overlap=1;

				insert_point_label5:for(int j=0; j < MAX_AOV_DIM; j++){
//#pragma HLS PIPELINE off




					float d = (regions[i_real].center[j] - regions[k_real].center[j]);
					distance += d*d;



					float d1 = regions[i_real].max[j] - regions[i_real].min[j];
					float d2 = regions[k_real].max[j] - regions[k_real].min[j];
					float ov;
					if(regions[i_real].min[j] < regions[k_real].min[j])
						ov = d1 - (regions[k_real].min[j] - regions[i_real].min[j]);
					else
						ov = d2 - (regions[i_real].min[j] - regions[k_real].min[j]);
					ov = ov < 0 ? 0 : ov;
					overlap *= ov;
				}
				float sc;
				//printf("b:%f d:%f o:%f = %f\n", behavior, distance, overlap, score);
				//if we are overlapping with another group, merge regardless.
				if(overlap > 0)
					sc = overlap;
				else
					//severely penalize groups where there is an interfering group
					sc = -distance; //negatively impact behavior.



				if(tmp_other_vld || sc > tmp_score){
					tmp_other = k_real;
					tmp_other_vld=true;
					tmp_score = sc;
				}


				if (k_real==MAX_REGIONS-1) {
					if(merge_1_vld || tmp_score > score){
						score = tmp_score;
						merge_1 = i_real;
						merge_1_vld=true;
						merge_2 = tmp_other;
					}
					tmp_score=0;
					tmp_other_vld=false;

					i_real++;
					k_real=i_real+1;
				} else {
					k_real++;
				}
			}

			//merge_regions(regions, merge_1, merge_2);
			//merge regions inlining
			insert_point_label6: for(int i=0; i < MAX_AOV_DIM; i++){
#pragma HLS PIPELINE off

				if(regions[merge_1].min[i] > regions[merge_2].min[i]){
					regions[merge_1].min[i] = regions[merge_2].min[i];
				}
				if(regions[merge_1].max[i] < regions[merge_2].max[i]){
					regions[merge_1].max[i] = regions[merge_2].max[i];
				}
				regions[merge_1].center[i] = (regions[merge_1].max[i] + regions[merge_1].min[i])/2.0;
			}


			//move everything over
			//			insert_point_label7:for(int i=merge_2; i < MAX_REGIONS-1; i++){
			////#pragma HLS PIPELINE off
			//#pragma HLS loop_tripcount min=1 max=15
			//				//if (i>=merge_2) {
			//					regions[i] = regions[i+1];
			//				//}
			//			}
			//if (merge_2!=(n_regions-1))
			regions[merge_2]=regions[MAX_REGIONS-1];
//			n_regions--;
		} else
			n_regions++;
	}
}


#define COMMAND_STOP 1
#define COMMAND_TEST 2
#define COMMAND_TRAIN 3

#define STATE_UNINITIALISED 0
#define STATE_READY 1

ap_uint<2> fsmstate=STATE_UNINITIALISED;

struct testpointDescriptorStr {
	ap_uint<16> uniId;
	ap_uint<8> checkId;
	ap_uint<8> executionId;
	float AOV[MAX_AOV_DIM];
}  __attribute__((packed, aligned(4)));

struct OutputStr {
	ap_uint<2> command;
	ap_uint<8> checkId;
	ap_uint<16> uniId;
	ap_uint<8> executionId;
	ap_uint<8> taskId;
	bool fault;
	float AOV[MAX_AOV_DIM];
};

struct controlStr {
	ap_uint<16> uniId;
	ap_uint<8> checkId;
	ap_uint<8> executionId;
	float AOV[MAX_AOV_DIM];
	ap_uint<8> taskId;
	char command;
	char gap0[2];
} __attribute__((packed, aligned(4)));

struct taskFailure {
	ap_uint<8> taskId;
	ap_uint<8> executionId;
};

#include "hls_math.h"
#define sizeOfInputData sizeof(float)*MAX_AOV_DIM


volatile void handle_outcome(volatile char errorInTask[MAX_TASKS], ap_uint<8> failedTaskExecutionId[MAX_TASKS], /*hls::stream< ap_uint<8>> &toScheduler,*/ volatile testpointDescriptorStr outcomeInRam[MAX_TASKS], volatile unsigned short *failedTask, hls::stream<OutputStr, 2> &source) {
#pragma HLS interface mode=ap_ctrl_hs port=return

	for (;;) {
#pragma HLS PIPELINE off

		OutputStr in=source.read();


		testpointDescriptorStr outcome;
		outcome.checkId=in.checkId;
		outcome.uniId=in.uniId;
		outcome.executionId=in.executionId;
		memcpy(&(outcome.AOV), &in.AOV, sizeOfInputData);

		if (in.command==COMMAND_STOP) {
			break;
		}

//		if (!(errorInTask[in.taskId] && failedTaskExecutionId[in.taskId]==in.executionId)) {
			errorInTask[in.taskId] = in.fault;
//
//			if (in.command==COMMAND_TEST)
				memcpy((testpointDescriptorStr*) (&outcomeInRam[in.taskId]), &outcome, sizeof(outcome));


//			if (in.fault) {
//				failedTaskExecutionId[in.taskId]=in.executionId;
//
//				*failedTask=(((unsigned short)in.taskId) | (((unsigned short)(in.executionId)) << 8));
//			}
//		}
	}
}


volatile void compute(region_t regions[MAX_CHECKS][MAX_REGIONS], ap_uint<4> n_regions[MAX_CHECKS], hls::stream<controlStr, 2> &source, hls::stream<OutputStr, 2> &dest) {

	controlStr in;
#pragma HLS ARRAY_PARTITION variable=in.AOV type=complete
#pragma HLS interface mode=ap_ctrl_hs port=return

	for (;;) {
#pragma HLS pipeline off

		//#pragma HLS pipeline II=300
		OutputStr out;
		in=source.read();
		out.checkId=in.checkId;
		out.uniId=in.uniId;
		out.executionId=in.executionId;
		out.taskId=in.taskId;
		out.command=in.command;
		memcpy((void*) &(out.AOV), (void*) &(in.AOV), sizeOfInputData);

		//			if (in.command!=COMMAND_TEST) {
		//				break;
		//			}
		if (in.command==COMMAND_TEST) {

			bool vld=is_valid(in.AOV);
			bool hasReg=hasRegion(regions[in.checkId], n_regions[in.checkId], in.AOV);//find_region(regions, n_regions, data) < 0 )
			bool fault = !(vld && hasReg);

			//if (error)
			out.fault=fault;
			dest.write(out);
			//		}
		} else if (in.command==COMMAND_TRAIN) {
			out.fault=false;
			dest.write(out);

			insert_point(regions[in.checkId],
					n_regions[in.checkId],
					in.AOV);
		} else if (in.command==COMMAND_STOP) {
			dest.write(out);
			break;
		}
	}
}

//void setProcessingState(volatile char* processing, bool value) {
//	if (value)
//		(*processing)=0xFF;
//	else
//		(*processing)=0x0;
//}

//controlStr* getInputPtr(volatile controlStr inputAOV[MAX_TASKS], volatile char* taskId) {
//	return (controlStr*)&(inputAOV[*taskId]);
//}

volatile void read_data(hls::stream<controlStr, 2> &dest, controlStr inputAOV[MAX_TASKS], volatile char* taskId, volatile char* copying) {

#pragma HLS interface mode=ap_ctrl_hs port=return

	for (;;) {


#pragma HLS PIPELINE off

//		if (*taskId) {


			controlStr destStr;
//			controlStr* ptr=getInputPtr(inputAOV, taskId);
//			setProcessingState(copying, true);



			//memcpy(&destStr, ptr, sizeof(controlStr));

			//									for (size_t i=0; i<sizeof(controlStr)/sizeof(unsigned long long); i++) {
			//						#pragma HLS PIPELINE
			//										*((volatile unsigned long long*) (&destStr))=*((volatile unsigned long long*) (inputAOV));
			//									}

//			for (size_t s=0; s<sizeof(controlStr); s++) {
//#pragma HLS pipeline
//				*((volatile unsigned char*) (&destStr)) = *(((volatile unsigned char*) ptr)+s);
//			}
//			destStr=inputAOV[*taskId];
			char tskId=*taskId;
			(*copying)=tskId | 0x1;
//			setProcessingState(copying, tskId | 0x1);
			memcpy(&destStr, &(inputAOV[tskId]), sizeof(controlStr));
//			destStr=*((controlStr*) &(inputAOV[tskId]));
//			destStr.checkId=inputAOV[tskId].checkId;
//			destStr.command=inputAOV[tskId].command;
//			destStr.taskId=inputAOV[tskId].taskId;
//			destStr.uniId=inputAOV[tskId].uniId;
//			destStr.executionId=inputAOV[tskId].executionId;
//
//			for (int i=0; i<MAX_AOV_DIM; i++) {
//				destStr.AOV[i]=inputAOV[tskId].AOV[i];
//			}

			//			destStr.checkId=inputAOV->checkId;
			//			destStr.command=inputAOV->command;
			//			destStr.executionId=inputAOV->executionId;
			//			destStr.taskId=inputAOV->taskId;
			//			destStr.uniId=inputAOV->uniId;
			//			for (int i=0; i<MAX_AOV_DIM; i++) {
			//				//#pragma HLS unroll
			//				destStr.AOV[i]=inputAOV->AOV[i];
			//			}

			if (destStr.command) {//==COMMAND_TRAIN || destStr.command==COMMAND_TEST || destStr.command==COMMAND_STOP) {
//				setProcessingState(copying, false);
				(*copying)=0x0;
			}
#pragma HLS DEPENDENCE type=intra variable=destStr dependent=true
#pragma HLS DEPENDENCE type=intra variable=destStr.AOV dependent=true
#pragma HLS DEPENDENCE type=intra variable=destStr.checkId dependent=true
#pragma HLS DEPENDENCE type=intra variable=destStr.command dependent=true
#pragma HLS DEPENDENCE type=intra variable=destStr.executionId dependent=true
#pragma HLS DEPENDENCE type=intra variable=destStr.taskId dependent=true
#pragma HLS DEPENDENCE type=intra variable=destStr.uniId dependent=true
			dest.write(destStr);

			if (destStr.command==COMMAND_STOP) {
				break;
			}
		}
//	}
}

void afterInit(controlStr inputAOV[MAX_TASKS], volatile char* taskId, /*char* readyForData,  char* copyInputAOV,*/
		volatile testpointDescriptorStr outcomeInRam[MAX_TASKS], /* hls::stream< ap_uint<8> > &toScheduler,*/ volatile char errorInTask[MAX_TASKS], ap_uint<8> failedTaskExecutionIds[MAX_TASKS], region_t regions[MAX_CHECKS][MAX_REGIONS], ap_uint<4> n_regions[MAX_CHECKS],  volatile unsigned short *failedTask, volatile char* copying
) {
#pragma HLS DATAFLOW disable_start_propagation
#pragma HLS interface mode=ap_ctrl_hs port=return

	hls::stream<controlStr, 2> sourceStream;
	hls::stream<OutputStr, 2> destStream;

	read_data(sourceStream, inputAOV, taskId, copying);
	compute(regions, n_regions, sourceStream, destStream);
	handle_outcome(errorInTask, failedTaskExecutionIds, outcomeInRam, failedTask, destStream);
}


static region_t regions[MAX_CHECKS][MAX_REGIONS]; //regions from the distribution
static ap_uint<4> n_regions[MAX_CHECKS];
static ap_uint<8> failedTaskExecutionIds[MAX_TASKS];


#define MODE_INIT 1
#define MODE_OUT 2
#define MODE_RUN 3

void FaultDetector(char accel_mode, volatile char* copying, controlStr inputData[MAX_TASKS], volatile char* taskId, volatile char errorInTask[MAX_TASKS], volatile testpointDescriptorStr lastTestDescriptor[MAX_TASKS], region_t trainedRegion_i, region_t *trainedRegion_o, ap_uint<8> IOCheckIdx, ap_uint<8> IORegionIdx, ap_uint<8> n_regions_i,  ap_uint<8> *n_regions_o, volatile  unsigned short *failedTask) {
#pragma HLS INTERFACE mode=ap_ctrl_hs port=return
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS interface mode= s_axilite port = accel_mode
#pragma HLS interface mode= m_axi port = inputData offset=slave

#pragma HLS interface mode= s_axilite port=copying
#pragma HLS interface mode= s_axilite port=taskId
#pragma HLS INTERFACE mode=ap_hs port=taskId

#pragma HLS interface mode= s_axilite port = trainedRegion_i
#pragma HLS interface mode= s_axilite port = trainedRegion_o
#pragma HLS interface mode= s_axilite port = IOCheckIdx
#pragma HLS interface mode= s_axilite port = IORegionIdx

#pragma HLS INTERFACE mode=ap_hs port=failedTask
#pragma HLS interface mode=s_axilite port = n_regions_i
#pragma HLS interface mode=s_axilite port = n_regions_o

#pragma HLS interface mode=s_axilite port = errorInTask
#pragma HLS INTERFACE mode=s_axilite port = lastTestDescriptor

#pragma HLS array_partition variable=regions dim=2 complete
//#pragma HLS array_partition variable=regions dim=3 complete


#pragma HLS reset variable=failedTaskExecutionIds


	if (accel_mode==MODE_INIT) {
		regions[IOCheckIdx][IORegionIdx]=trainedRegion_i;
		n_regions[IOCheckIdx]=n_regions_i;
	} else if (accel_mode==MODE_OUT) {
		*trainedRegion_o=regions[IOCheckIdx][IORegionIdx];
		*n_regions_o=n_regions[IOCheckIdx];
	} else if (accel_mode==MODE_RUN) {
		afterInit(inputData, taskId, lastTestDescriptor, errorInTask, failedTaskExecutionIds, regions, n_regions, failedTask, copying);
	}
}


//int find_region(const region_t regions[MAX_REGIONS], const ap_uint<8> n_regions, const float d[MAX_AOV_DIM]){
//
//	int idx = -1;
//	float score = -1;
//	for(int i=0; i < n_regions; i++){
//#pragma HLS loop_tripcount min=0 max=16
//#pragma HLS PIPELINE II=4
//		//#pragma HLS unroll
//		//		if (i>=n_regions)
//		//			break;
//
//		bool is_idx = true;
//		float tmp_score = 0;
//		float dist = 0;
//		float area = 0;
//		find_region_label1:for(int j=0; j < MAX_AOV_DIM; j++){
//			//compute the distance and scale.
//			//#pragma HLS PIPELINE II=1
//			float ldist = (d[j] - regions[i].center[j]);
//			float hdist = (regions[i].max[j] - regions[i].center[j]);
//			float absdist = ldist*ldist;
//			float scale = hdist*hdist;
//			area += scale;
//			dist += absdist > scale ? absdist : scale;
//			//determine if we're out of the bound
//			if(regions[i].min[j] > d[j] || regions[i].max[j] < d[j])
//				is_idx = false;
//		}
//		//used to just be smallest area.
//		tmp_score = 1/area;
//		if(is_idx && (idx < 0 || tmp_score > score)){
//			idx = i;
//			score = tmp_score;
//		}
//	}
//	return idx;
//}

//void update_train_regions(region_t regions[MAX_REGIONS], const int id, const float val[MAX_AOV_DIM] ) {// , bool is_acc){
//#pragma HLS inline
//	//TODO for logging
//
//	//forget behavior
//	//	for(int i=0; i < n_regions; i++){
//	//		regions[i]->stats.update_accuracy_rate(id == i, is_acc);
//	//	}
//	//we do not have a region.
//
//	//this->env->update_accuracy_rate(id < 0, false);
//	//this->stats->update_accuracy_rate(is_acc); //is accept, is error
//
//	//if(id < 0 || !is_acc) return; //if outside of region, or error.
//	//we have a region
//	//update boundaries to include point.
//
//	update_train_regions_label3:for(int i=0; i < MAX_AOV_DIM; i++) {
//		if(val[i] > regions[id].max[i]) regions[id].max[i] = val[i];
//		else if(val[i] < regions[id].min[i]) regions[id].min[i] = val[i];
//		regions[id].center[i] = (regions[id].max[i] + regions[id].min[i])/2.0;
//	}
//}

//bool compare(const float data[MAX_AOV_DIM], const float data_key[MAX_AOV_DIM]){
//	for(int i=0; i < MAX_AOV_DIM; i++){
//		if(fabs(data_key[i] - data[i]) > thresh){
//			return false;
//		}
//	}
//	return true;
//}

//find region, ie find the closest region with similar output, error
//characteristics, returning score.
/* We want to merge nearby regions with a good score
 *
 */
//float score_region(const region_t * r1, const region_t * r2) {
//#pragma HLS inline
//	float distance = 0;
//	//compute the distance
//	for(int j=0; j < MAX_AOV_DIM; j++){
//#pragma HLS unroll
//		float d = (regions[idx].center[j] - regions[i].center[j]);
//		distance += d*d;
//	}
//	float overlap;
//	overlap=1;
//	for(int i=0; i < MAX_AOV_DIM; i++){
//#pragma HLS unroll
//
//		float d1 = regions[idx].max[i] - regions[idx].min[i];
//		float d2 = regions[i].max[i] - regions[i].min[i];
//		float ov;
//		if(regions[idx].min[i] < regions[i].min[i])
//			ov = d1 - (regions[i].min[i] - regions[idx].min[i]);
//		else
//			ov = d2 - (regions[idx].min[i] - regions[i].min[i]);
//		ov = ov < 0 ? 0 : ov;
//		overlap *= ov;
//	}
//	float score=0;
//	//printf("b:%f d:%f o:%f = %f\n", behavior, distance, overlap, score);
//	//if we are overlapping with another group, merge regardless.
//	if(overlap > 0) return overlap;
//	//severely penalize groups where there is an interfering group
//	score -= distance; //negatively impact behavior.
//
//	return score;
//}

//int find_closest_region(const region_t regions[MAX_REGIONS], const ap_uint<8> n_regions, const int idx, float * score){
//	//#pragma HLS inline
//#pragma HLS PIPELINE II=32
//
//	float bestscore=0.0;
//
//	int id = -1;
//	for(int i=0; i < MAX_REGIONS; i++){
//#pragma HLS unroll
//
//		if (i>=n_regions)
//			break;
//		if(i != idx) {
//			//printf("score [%d,%d]:", idx, i);
//
//			float distance = 0;
//			float overlap=1;
//
//			for(int j=0; j < MAX_AOV_DIM; j++){
//#pragma HLS unroll
//
//
//				float d = (regions[idx].center[j] - regions[i].center[j]);
//				distance += d*d;
//
//
//
//				float d1 = regions[idx].max[j] - regions[idx].min[j];
//				float d2 = regions[i].max[j] - regions[i].min[j];
//				float ov;
//				if(regions[idx].min[j] < regions[i].min[j])
//					ov = d1 - (regions[i].min[j] - regions[idx].min[j]);
//				else
//					ov = d2 - (regions[idx].min[j] - regions[i].min[j]);
//				ov = ov < 0 ? 0 : ov;
//				overlap *= ov;
//			}
//			float sc;
//			//printf("b:%f d:%f o:%f = %f\n", behavior, distance, overlap, score);
//			//if we are overlapping with another group, merge regardless.
//			if(overlap > 0)
//				sc = overlap;
//			else
//				//severely penalize groups where there is an interfering group
//				sc = -distance; //negatively impact behavior.
//
//
//			//					score_region(&(regions[idx]), &(regions[i]));
//
//
//			if(id == -1 || sc > bestscore){
//				id = i;
//				bestscore = sc;
//			}
//		}
//	}
//	*score=bestscore;
//	return id;
//}

////merge r2 into r1
//void merge_regions(region_t regions[MAX_REGIONS], const int id1, const int id2){
//#pragma HLS inline
//	for(int i=0; i < MAX_AOV_DIM; i++){
//#pragma HLS unroll
//
//		if(regions[id2].min[i] < regions[id1].min[i]){
//			regions[id1].min[i] = regions[id2].min[i];
//		}
//		if(regions[id2].max[i] > regions[id1].max[i]){
//			regions[id1].max[i] = regions[id2].max[i];
//		}
//		regions[id1].center[i] = (regions[id1].max[i] + regions[id1].min[i])/2.0;
//	}
//
//	//move everything over
//	for(int i=0; i < MAX_REGIONS-1; i++){
//		#pragma HLS unroll
//
//		if (i>=id2) {
//			regions[i] = regions[i+1];
//		}
//	}
//}
