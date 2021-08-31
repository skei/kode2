#ifndef kode_list_included
#define kode_list_included
//----------------------------------------------------------------------

/*

doubly-linked 'invasive' list..

  a templates (ala array)data-container)
  b multiple inheritance (ala widgetListener)
  * non-invasive list.. nodes = next, prev, ptr

*/

//----------------------------------------------------------------------

class KODE_ListNode {
private:
  KODE_ListNode*  MPrev = KODE_NULL;
  KODE_ListNode*  MNext = KODE_NULL;
public:
  KODE_ListNode() {}
  virtual ~KODE_ListNode() {}
public:
  KODE_ListNode*  prev() { return MPrev; }
  KODE_ListNode*  next() { return MNext; }
  void prev(KODE_ListNode* ANode) { MPrev = ANode; }
  void next(KODE_ListNode* ANode) { MNext = ANode; }
};

//----------------------------------------------------------------------

class KODE_List {

//------------------------------
private:
//------------------------------

  KODE_ListNode*  MHead = KODE_NULL;
  KODE_ListNode*  MTail = KODE_NULL;
  uint32_t        MSize = 0;

//------------------------------
public:
//------------------------------

  KODE_List() {}
  virtual ~KODE_List() {}

//------------------------------
public:
//------------------------------

public:

  KODE_ListNode*  head() { return MHead; }
  KODE_ListNode*  tail() { return MTail; }

  void head(KODE_ListNode* ANode) { MHead = ANode; }
  void tail(KODE_ListNode* ANode) { MTail = ANode; }

  //----------

  void clear() {
    KODE_ListNode* node = MHead;
    while (node) {
      KODE_ListNode* next = node->next();
      //KTimeline_Segment* segment = (KTimeline_Segment*)node;
      delete node;
      node = next;
    }
  }

  //----------

  bool isEmpty() {
    return (MHead == KODE_NULL);
  }

  //----------

  bool isNotEmpty() {
    return (MHead != KODE_NULL);
  }

  //----------

  bool hasNode(KODE_ListNode* ANode) {
    KODE_ListNode* node = MHead;
    while (node) {
      if (node == ANode) return true;
      node = node->next();
    }
    return false;
  }

  //----------

  void insertHead(KODE_ListNode* ANode) {
    MSize += 1;
    if (MHead) {
      // -H -> -*H
      MHead->prev(ANode);
      ANode->next(MHead);
      ANode->prev(KODE_NULL);
      MHead = ANode;
    }
    else {
      // - -> -*-
      ANode->next(KODE_NULL);
      ANode->prev(KODE_NULL);
      MTail = ANode;
      MHead = ANode;
    }
  }

  //----------

  void insertTail(KODE_ListNode* ANode) {
    MSize += 1;
    if (MTail) {
      // T- -> T*-
      ANode->next(KODE_NULL);
      ANode->prev(MTail);
      MTail->next(ANode);
      MTail = ANode;
    }
    else {
      // empty list
      // - -> -*-
      ANode->next(KODE_NULL);
      ANode->prev(KODE_NULL);
      MTail = ANode;
      MHead = ANode;
    }
  }

  //----------

  void removeHead() {
    MSize -= 1;
    if (MHead) {
      // -H?
      if (MHead==MTail) {
        // -H- -> -
        MHead = KODE_NULL;
        MTail = KODE_NULL;
      }
      else {
        // -Hn -> n-
        MHead->next()->prev(KODE_NULL);
        MHead = MHead->next();
      }
    }
  }

  //----------

  void removeTail() {
    MSize -= 1;
    if (MTail) {
      // ?T-
      if (MTail==MHead) {
        // -T- -> -
        MHead = KODE_NULL;
        MTail = KODE_NULL;
      }
      else {
        // pT- -> p-
        MTail->prev()->next(KODE_NULL);
        MTail = MTail->prev();
      }
    }
  }

  //----------

  void insertBefore(KODE_ListNode* ANode, KODE_ListNode* ANewNode) {
    MSize += 1;
    if (ANode->prev()) {
      // pN?
      //if (ANode->next()) {
        // pNn -> p*Nn
        ANode->prev(ANewNode);
        ANewNode->next(ANode);
        ANewNode->prev(ANode->prev());
        ANode->prev()->next(ANewNode);
      //}
      //else {
      //  // pN- -> p*N-
      //  // (similar to the above)
      //  ANode->prev(ANewNode);
      //  ANewNode->next(ANode);
      //  ANewNode->prev(ANode->prev());
      //  ANode->prev()->next(ANewNode);
      //}
    }
    else {
      // -N?
      //if (ANode->next()) {
        // -Nn -> -*Nn
        ANode->prev(ANewNode);
        ANewNode->next(ANode);
        ANewNode->prev(KODE_NULL);
        MHead = ANewNode;
      //}
      //else {
      //  // -N- -> -*N-
      //  // (similar to the above)
      //  ANode->prev(ANewNode);
      //  ANewNode->next(ANode);
      //  ANewNode->prev(KODE_NULL);
      //  MHead = ANewNode;
      //}
    }
  }

  //----------

  void insertAfter(KODE_ListNode* ANode, KODE_ListNode* ANewNode) {
    MSize += 1;
    if (ANode->next()) {
      // ?Nn
      //if (ANode->prev()) {
        // pNn -> pN*n
        ANode->next()->prev(ANewNode);
        ANewNode->next(ANode->next());
        ANewNode->prev(ANode);
        ANode->next(ANewNode);
      //}
      //else {
      //  // -Nn -> -N*n
      //  // (similar to the above)
      //  ANode->next()->prev(ANewNode);
      //  ANewNode->next(ANode->next());
      //  ANewNode->prev(ANode);
      //  ANode->next(ANewNode);
      //}
    }
    else {
      // ?N-
      //if (ANode->prev()) {
        // pN- -> pN*-
        ANewNode->next(KODE_NULL);
        ANewNode->prev(ANode);
        ANode->next(ANewNode);
        MTail = ANode;
      //}
      //else {
      //  // -N- -> -N*-
      //  // (similar to the above)
      //  ANewNode->next(KODE_NULL);
      //  ANewNode->prev(ANode);
      //  ANode->next(ANewNode);
      //  MTail = ANode;
      //}
    }
  }

  //----------

  void removeNode(KODE_ListNode* ANode) {
    MSize -= 1;
    if (ANode->next()) {
      // ?Nn
      if (ANode->prev()) {
        // pNn -> pn
        ANode->next()->prev(ANode->prev());
        ANode->prev()->next(ANode->next());
      }
      else {
        // -Nn -> -n
        ANode->next()->prev(KODE_NULL);
        MHead = ANode->next();
      }
    }
    else {
      // ?N-
      if (ANode->prev()) {
        // pN- -> p-
        ANode->prev()->next(KODE_NULL);
        MTail = ANode->prev();
      }
      else {
        // -N- -> -
        MHead = KODE_NULL;
        MTail = KODE_NULL;
      }
    }
  }

  //----------

  //void sortNodes() {
  //}

  //void findNode() {
  //}

  //void swapNodes(Kode_ListNode* ANode1, Kode_ListNode* ANode2) {
  //}

  //void moveNodeToHead(Kode_ListNode* ANode) {
  //}

  //void moveNodeToTail(Kode_ListNode* ANode) {
  //}

  //----------

};

//----------------------------------------------------------------------
#endif
