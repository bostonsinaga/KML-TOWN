#ifndef __MINI_TOOL__TPP__
#define __MINI_TOOL__TPP__

template<typename T>
int mini_tool::isPrimitiveInsideVector(
  std::vector<T> &dataVec,
  T dataTest
) {
  int ctr = 0;
  for (auto &data : dataVec) {
    if (data == dataTest) return ctr;
    ctr++;
  }
  return -1;
}

/**
 * Return examples:
 * {
 *   {0, 1, 2}  -> duplicate on
 *   {3, 4}        container[2] and test[5]
 * }
 * {
 *   {0}        -> duplicate off
 *   {3}           container[2] and test[5]
 * }
 */
template<typename T>
std::vector<std::vector<int>> mini_tool::vectorIntersectsVector(  // pass 1D vector only
  std::vector<T> &containerVec,
  std::vector<T> &testVec,
  bool isTestDuplicate
) {
  bool isIntersected = false;
  std::vector<std::vector<int>> intersectIndexesVec(containerVec.size(), std::vector<int>{});

  // test vector //

  int containerCtr = 0;
  for (auto &containerVal : containerVec) {

    int testCtr = 0;
    for (auto &testVal : testVec) {
      if (containerVal == testVal) {

        if (!isIntersected) isIntersected = true;
        intersectIndexesVec.at(containerCtr).push_back(testCtr);
        if (!isTestDuplicate) break;
      }
      testCtr++;
    }
    containerCtr++;
  }

  // cleaning empty first dimension
  if (isIntersected) {
    std::vector<std::vector<int>> intersectIndexesVec_buffer;
    
    for (auto &indexes : intersectIndexesVec) {
      if (indexes.size() > 0) {
        intersectIndexesVec_buffer.push_back(indexes);
      }
    }

    intersectIndexesVec = intersectIndexesVec_buffer;
  }
  // return check
  else return std::vector<std::vector<int>>{};
  return intersectIndexesVec;
}

template<typename T>
std::vector<T> mini_tool::collapse2DTo1DVector(std::vector<std::vector<T>> &vec_in) {
  std::vector<T> newVec;

  for (auto &vec1D : vec_in) {
    if (vec1D.size() > 0) {
      newVec.push_back(vec1D.front());
    }
  }

  return newVec;
}

#endif // __MINI_TOOL__TPP__