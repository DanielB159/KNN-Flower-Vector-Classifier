#ifndef SLCT_H
#define SLCT_H
#include <vector>

namespace slct {
    /// @brief abstract template class for a comperator function class.
    /// @tparam T object type to compare
    template <class T>
    class Comparator {
        public:
            /// @brief Compare two objects of T type
            /// @param obj1 first object
            /// @param obj2 second object
            /// @return  boolean of obj1 < obj2
            bool operator()(const T& obj1, const T& obj2);
    };  //must createa specific implementations for this

    /// @brief swap two objects T.
    /// @tparam T type of object to swap.
    /// @param obj1 reference to first object
    /// @param obj2 reference to second object
    template <class T>
    void swap(T& obj1, T& obj2);

    /// @brief Partition segment of a c array.
    /// @tparam T type of objects in array
    /// @param array pointer to the beginnning of the array.
    /// @param cmp Comparator<T> class defining the comparison.
    /// @param leftIndex leftmost index of the segment.
    /// @param rightIndex rightmost index of the segment.
    /// @param pivotIndex index for the pivot element to partition based on.
    /// @return index of index element after the partition.
    template <class T>
    int partition(T* array, Comparator<T> cmp, int leftIndex, int rightIndex, int pivotIndex);

    /// @brief Select algorithm for finding k-th size element. 
    ///     in the course of it partitions the array for it. O(n) time
    /// @tparam T type of objects in array.
    /// @param array pointer to the beginning of array.
    /// @param size size of array.
    /// @param cmp Comparator<T> class defining comparison.
    /// @param k k defined by the algorithm.
    /// @return index of k-th size element.
    template <class T>
    int select(T* array, int size, Comparator<T> cmp, int k);

    /// @brief finds median of given segment in array. O(n^2) time
    /// @tparam T type of objects in array.
    /// @param array pointer to first object in array.
    /// @param cmp Comparator<T> class defining comparison.
    /// @param leftIndex leftmost index of the segment.
    /// @param rightIndex rightmost index of the segment.
    /// @return median of the segment.
    template <class T>
    int median(T* array, Comparator<T> cmp, int leftIndex, int rightIndex);

    /// @brief Select algorithm for finding k-th size element in segment of c array.
    /// @tparam T type of objects in array
    /// @param array pointer to first object in array.
    /// @param cmp Comparator<T> class defining comparison.
    /// @param k k defined by the algorithm.
    /// @param leftIndex leftmost index of the segment.
    /// @param rightIndex rightmost index of the segment.
    /// @return index of k-th size element.
    template <class T>
    int _select(T* array, Comparator<T> cmp, int k, int leftIndex, int rightIndex);  //leftIndex + k
}

template<class T>
void slct::swap(T& obj1, T& obj2) {
    //swap two objects using their move constructors.
    T temp = std::move(obj1);
    obj1 = std::move(obj2);
    obj2 = std::move(temp);
    return;
}

template <class T>
int slct::partition(T* array, Comparator<T> cmp, int leftIndex, int rightIndex, int pivotIndex) {
    //remember pivot value and move it's element to the right of the segment
    T pivotValue = array[pivotIndex];
    swap(array[pivotIndex], array[rightIndex]);
    int storeIndex = leftIndex;
    for (int i = leftIndex; i < rightIndex; i++) {
        //move all smaller elements to the left size
        if (cmp(array[i], pivotValue) == true) {    //array[i] < pivotValue
            swap(array[storeIndex], array[i]);
            storeIndex++;
        }
    }
    //move pivot to it's final position
    swap(array[rightIndex], array[storeIndex]);
    return storeIndex;
}

template <class T>
int slct::median(T* array, Comparator<T> cmp, int leftIndex, int rightIndex) {
    //calculate size
    int size = rightIndex - leftIndex + 1;
    //primitive sorting, searching for minimum value and moving it to the left
    for (int j = 0; j < size; j++) {
        int minIndex = leftIndex + j;
        T minValue = array[leftIndex + j];
        for (int i = leftIndex + j; i <= rightIndex; i++) {
            if (cmp(array[i], minValue) == true) {  //array[i] < minValue
                minIndex = i;
                minValue = array[i];
            }
        }
        swap(array[minIndex], array[leftIndex + j]);
    }

    //return middle index, this would be the median element.
    return leftIndex + (size/2);
}


template <class T>
int slct::select(T* array, int size, Comparator<T> cmp, int k) {
    //call for the more general select algorithm with given size
    int kIndex = _select<T>(array, cmp, k, 0, size - 1); 
    return partition(array, cmp, 0, size - 1, kIndex);
}

template <class T>
int slct::_select(T* array, Comparator<T> cmp, int k, int leftIndex, int rightIndex) {
    //exit condition, if segment is size 1
    if (rightIndex == leftIndex) {
        return rightIndex;
    }

    //find medians of medians, move them to left of segment and remember how many there were
    int medians = 0;
    for (int i = leftIndex; i <= rightIndex; i += 5) {
        //define segments of size <= 5 to find the median of
        int minRight = i + 4 > rightIndex ? rightIndex : i + 4;
        int medIndex = median(array, cmp, i, minRight);
        swap(array[leftIndex + medians], array[medIndex]);
        medians++;
    }

    //get median of medians using recursive call for n/2 + 1 th size element.
    int medofmedsIndex = _select<T>(array, cmp, leftIndex + (medians/2) + 1, leftIndex, leftIndex + medians - 1);
    //partition based on it
    medofmedsIndex = partition(array, cmp, leftIndex, rightIndex, medofmedsIndex);

    if (medofmedsIndex == k - 1) {
        //found the correct element, return it's index
        return medofmedsIndex;
    }
    if (medofmedsIndex > k - 1) {
        //answer is on left side, disregard right side
        return _select<T>(array, cmp, k, leftIndex, medofmedsIndex - 1);
    }
    // answer is on right side, check right side
    return _select<T>(array, cmp, k, medofmedsIndex + 1, rightIndex);

}

#endif