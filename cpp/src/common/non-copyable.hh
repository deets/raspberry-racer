#ifndef NON_COPYABLE_HH
#define NON_COPYABLE_HH


class NonCopyAble {

private:
  // prevent copying
  NonCopyAble(const NonCopyAble&);
  NonCopyAble & operator=(const NonCopyAble&);

public:
  NonCopyAble() {};
};

#endif
