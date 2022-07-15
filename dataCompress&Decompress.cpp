/*
 Code Design Test: Data Compression Design
 Jason Jicheng Zhao
 Tue, July 12, 2022
 
 
 Basic design :
 1. For this assignment, I decided to use C++, and without using STLs. I am confident with using STL too.  However, for this task, I think we do not have to use STL, and the position I am applying for, is also more embedded C focus, so this might be a good way to approach this assignment.
 1.this algorithm will compress the same value repeated in the series into 2 integers(value, #number). ex. 1, 1, 1 ==> 1,3
 2. Just to make sure that we don't accidentally expend the data. if it is a unique number, we only print out the value. ex. 1,2,3,3,3 ==> 1,2,3,3
 4. Since in CPP, the array could only hold one type of data and we would like to have a lossless compression, we must distinguish the value and #number. to do so I decided to use the negative sign. However, this method also has a limitation size of 128. If it spills over, we will write -128 and the reminder. ex. 257 0s => 0,-128,-128,-1
 3. There are 3 pointers for the algorithm, i,j, write+p. j start iterating the array, once it meets a different value, write_p will write the data with the value or ( value and #number). I catch up.
 4 The limitation for this algorithm is it will not compress that well if there is not same value repeated in the series for more than 2 times. But it would not large the size of the data.
 5. runtime: O(2n) iterate through the whole array to copy or run the algorithm.
 6. space complexity: O(1) no complex data struct, – constant complexity
 
 
Remarks:
1.  The implementation was only used with the std library, without using STLs. it saves a little bit more space and runtime are a little faster in theory. I personally feel it is a little bit more challenging, so I decided to do it this way.
 
2. However, when executing the algorithm, we are overwriting the array, and the useless space is filled with garbage data, this could potentially lead to code vulnerabilities. Possible ways to fix it: 2. Create a different array just for the compressed data, and return the pointer and the size.

2. If we just pass a pointer of the original data address, and overwrite the value. Technically, we will save the pointer space, and avoid the O(n) for copying the data to the new array. But we will lose the original data.  In this code, we were able to Deallocate Heap memory.

3. This is when stls come in handy because I can easily resize my container class.
 */

#include <cstdio>
#include <cstdint>

/**
 * compress the data
 * @param data_ptr Array Container where original are summed.
 * @param data_size lengh of the array.
 * @return new size
 */
size_t byte_compress( int8_t * data_ptr, size_t data_size){
    puts("byte_compress:");

    size_t compress_size;
    int8_t * write_p = data_ptr;
    int8_t * i = data_ptr;
    size_t count_i = 0;
  
    while(count_i<data_size){
        size_t count_j = 0;
        size_t over_fLow = 0;
        int8_t * j=i;
        
        while ((count_j < data_size) && (*(j++) == *i )){
            count_j++;
        }
        
        *write_p = *i;
        write_p++;
        
        for (size_t x =0; x<count_j; x++){
            i++;
        };
        
        count_i += count_j;
        
        
        //too many same value repeated in the series
        while (count_j>128){
            
            //specilly case since cout_j would be 1 and that will not be catch
            if (count_j == 129){
                *write_p = -128;
                write_p++;
                *write_p = -1;
                write_p++;
                over_fLow++;
                count_j -= 128;
                break;
            }
            
            *write_p = -128;
            write_p++;
            over_fLow++;
            count_j -= 128;

        }
        
        if (count_j>1){
            *write_p = - count_j;
            write_p++;
            ;
        }
         

    }
    
    compress_size = write_p - data_ptr;
    printf("compress_size: %zd\n",compress_size);
    
    return compress_size;
}




/**
 * decompress the data. while positive, wirte it out. if negeative, wirte  out the last positive value -negeative-1 times.
 * @param data_ptr Array Container where original are summed.
 * @param data_size lengh of the array.
 * @return decompress size
 */
size_t byte_decompress(int8_t * data_ptr, size_t data_size){
  
    puts("\nbyte_decompress");
   
    size_t decompress_size;
    int8_t * write_p = data_ptr+data_size;
    int8_t * decompress_i = data_ptr;
    size_t count_i = 0;
    
    while(count_i<data_size){
        size_t count_j = 0;
        int8_t * j = decompress_i;
        size_t repeats = 0;
        int8_t repeat_value;
        int8_t overflow =1;
        
        if ( *data_ptr <0){
            puts("error format result ");
            break;
        }
        //while positive, write it out to new array
        while ((count_j < data_size) && *(j)>=0){
            *write_p = *j;
            write_p++;
            
            count_j++;
            j++;
        }
        //when it meet a positive, get the repeat vaule and time,
        if ((count_j < data_size) && *(j)<0) {
            repeats = -*j;
            repeat_value = *(j-1);
            
            //if multiple negitive values in a row, update the repeat time
            while (*(++j) < 0) {
                repeats += -*j;
                count_j++;
                overflow++;
            }
            
            //write out the repeat value for repeat time
            for (size_t x =0; x<repeats-1; x++) {
                *write_p = repeat_value;
                write_p++;
            }
            
        } else {
            puts("wrong data ");
            break;

        }
        
        
        //bring i forward
        count_i += count_j+1;
        decompress_i += count_j+1;
        
    }
    
    decompress_size = write_p - data_ptr-data_size;
    
    printf("size decompress_size is at: %zd\n", decompress_size);
    
    return decompress_size;
}


//basic print function
void print_buffer (int8_t * data_ptr,size_t data_size ){
    
    for ( size_t i = 0; i < data_size; i++ ){
            printf("%d ",data_ptr[i]);
        }
    puts("");
}


int main(){
    //return len: sum(1,1,2,2,2,2,1,1,2,2) =16
    int8_t data[] = { 0x03, 0x74, 0x04, 0x04, 0x04, 0x35, 0x35, 0x64,
        0x64, 0x64, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x56, 0x45, 0x56, 0x56, 0x56, 0x09, 0x09, 0x09};

    size_t data_size =sizeof(data)/sizeof(data[0]);
    
    //Allocate Heap memory array we will working on
    int8_t * data_ptr = new int8_t[data_size];
    for (size_t i = 0; i < data_size; i++)
    {
          data_ptr[i] = data[i];
    }
    
    puts("setup ");
    //print out the original
    printf("size of the data:%zd\n", sizeof(data));
    print_buffer(data_ptr,data_size);
    puts("");

    size_t new_size  = byte_compress( data_ptr, data_size); // after we know that the compression data is store at pionter data_ptr, and length of the new_size.
    
    //creat a new array for compress data
    int8_t * compress_array = new int8_t [new_size];
    for (size_t i =0; i < new_size; i++){
      compress_array[i] = (data_ptr[i]);
    }
    
    //print out the compress result
    print_buffer(compress_array,new_size);
    
    
    ///start of the decompress
    size_t full_size =byte_decompress(compress_array, new_size );
    
    //creat a new array for decompress data
    int8_t * decomp_array = new int8_t [full_size];
    for (size_t i =0; i < full_size; i++){
        decomp_array[i] = (compress_array[i+new_size]);
    }
        
    print_buffer(decomp_array,full_size);
    // Deallocate Heap memory
    delete []compress_array;
    ///end of the decompress
  
    
    // Deallocate Heap memory
    delete []data_ptr;

    puts("\nend");
    return 0;
}


