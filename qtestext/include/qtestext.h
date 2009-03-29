/*
 Copyright (C) 2009  Gilbert Jeiziner
 Licensed under GNU GPL v2.
 http://www.gnu.org/licenses/gpl.html
 See LICENSE for license details.
*/
#ifndef QTESTEXT_H
#define QTESTEXT_H

/*
  QTextExt is a simple extension to the Qt Test Module.
  Consulte the Qt documentation on how to create tests
  using the Qt Test Module. It's heavily inspired
  by a similar by a similar project called QtTestUtil
  http://git.el-tramo.be/browse/qttestutil.git/

  It allows the registration of tests to run in test
  suits. Suits are organized in a hierarchy. A runner
  then allows to executed registered tests, executing
  only parts of the hierarchy, if desired.

  The hierarchy is build using names. Each node of
  the hierarchy can contain none, one or several tests
  to be executed.

  Simply add a QTESTEXT_ADDTO_SUITE for every test-class you
  want to run, typically this is added in your *.cpp file of
  the test class. You also need to include the *moc file of
  the test class (see Qt Test Module documentation for
  details)

  Sample usage:
  Let's have a couple of test classes with their registered names.
  QTESTEXT_ADD_TO(TestSample1,first);
  QTESTEXT_ADD_TO(TestSample2,testing);
  QTESTEXT_ADD_TO(TestSample3,testing_sample);
  QTESTEXT_ADD_TO(TestSample4,testing_sample_other);
  QTESTEXT_ADD_TO(TestSample5,testing_sample_other);
  QTESTEXT_ADD_TO(TestSample6,testing_sample_yetanother);
  Note that the hierarchy name has to be specified without
  quotes!
  QTESTEXT_ADD_TO(TestSampleXXX,"dummy"); will spew
  a compiler error.
  As you can see with sample 4 and 5, tests can be added to
  the same node in the hierarchy. Also, it's possile to add the
  same test class to different nodes in the hierarchy:
  QTESTEXT_ADD_TO(TestSample2,"second"); is a valid definition.
  It's not possible to add the same class to the same hierarchy
  node:
  QTESTEXT_ADD_TO(TestSample6,"testing_sample_yetanother");
  is NOT valid. It will spew an error on complie time.

  The '_' seperator is used because this library uses macro
  expansion to create unique variable names.

  With TestSuite::getInstance()->run(...), you can run one or several tests.
  The function will return the number of failed tests. It's typically used in the
  main(...) function of your test application.

  An asterix can be used to run all children (and their children)
  from a node.
  TestSuite::getInstance()->run(...); will run ALL tests (provided for convenience)
  TestSuite::getInstance()->run("*",...); will run ALL tests
  TestSuite::getInstance()->run("",...); will run ALL tests (provided for convenience)
  TestSuite::getInstance()->run("first",...); will run QTestSample1
  TestSuite::getInstance()->run("testing",...); will run QTestSample2
  TestSuite::getInstance()->run("testing_*",...); will run Sample2 to Sample6
  TestSuite::getInstance()->run("testing_sample",...); will run Sample3
  TestSuite::getInstance()->run("testing_sample_*",...); will run Sample3 to Sample6
  TestSuite::getInstance()->run("testing_sample_other",...); will run Sample4 and Sample 5

  For a sample on the usage of this small library, see the
  samples directory.
*/

#include "testFacade.h"

#define QTESTEXT_ADD_TO(TestClass,Hierarchy) static qtestext::TestFacade<TestClass> TestClass##Hierarchy(QString(#Hierarchy))

#endif // QTESTEXT_H
