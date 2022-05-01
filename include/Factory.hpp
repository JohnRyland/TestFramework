/*
   Factory Pattern
   Created by John Ryland on 17/3/19
   Copyright (C) 2019-2022 John Ryland. All rights reserved.
*/

#pragma once

#include <atomic>

/// This creates a linked-list of T type objects when a new T is
/// registered by declaring a FactoryItem global variable. This is
/// used by the unit test framework to register and collect a list
/// of all tests.
///
/// This might also be useful for registering the modules.
///
/// While this class is not the Factory of the factory pattern, these
/// factory items can be used by a factory class to instantiate instances.
///
/// For example in the context of unit testing, each unit test will
/// have an associated FactoryItem wrapper, and the unit test framework
/// will be the factory creating instances of unit tests to run.
template <typename T>
struct FactoryItem
{
  using ItemT = FactoryItem<T>;

  /// Example usage:
  ///
  ///     Test test = { ... };
  ///     FactoryItem<Test> registerTest(test);
  /// or just:
  ///     FactoryItem<Test> registerTest({ ... });
  ///
  /// This will add 'test' to a list of Test items
  /// which can be iterated with:
  ///
  ///     for (auto test : FactorItem<Test>::items())
  ///
  FactoryItem(T a_value)
    : m_value(a_value)
    , m_next(getFactoryHead())
  {
    getFactoryHead(this);
  }

  /// This can be used to begin C++11 range-based iteration, for example:
  ///
  ///     for (auto test : FactorItem<Test>::items())
  ///
  static ItemT& items()                        { return *getFactoryHead(); }

  class Iterator
  {
  public:
    Iterator(ItemT* obj) : m_object(obj)       { }
    T& operator*() const                       { return m_object->m_value; }
    bool operator!=(const Iterator& it) const  { return m_object != it.m_object; }
    Iterator& operator++()                     { m_object = m_object->m_next; return *this; }
  private:
    ItemT* m_object;
  };

  // Support C++11 range-based looping over these
  Iterator begin()                             { return Iterator(&items()); }
  Iterator end()                               { return Iterator(nullptr); }

private:
  friend class ItemT::Iterator;

  T        m_value;
  ItemT*   m_next;
  static ItemT* getFactoryHead(ItemT* newValue = nullptr);
};


// It shouldn't matter, but the compiler for XCode needed this not to be inline in the definition
template <typename T>
FactoryItem<T>* FactoryItem<T>::getFactoryHead(FactoryItem<T>* newValue)
{
  static std::atomic<ItemT*> s_factoryHead(nullptr);
  if (newValue)
  {
    s_factoryHead = newValue;
  }
  return s_factoryHead;
}

