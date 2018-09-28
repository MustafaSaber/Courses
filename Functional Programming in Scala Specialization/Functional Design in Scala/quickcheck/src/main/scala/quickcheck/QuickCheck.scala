package quickcheck

import common._

import org.scalacheck._
import Arbitrary._
import Gen._
import Prop._

import Math.min
abstract class QuickCheckHeap extends Properties("Heap") with IntHeap {

  lazy val genHeap: Gen[H] = for {
    n <- arbitrary[A]
    h <- frequency((1, Gen.const(empty)), (9 , genHeap))
  } yield insert(n ,h)
  implicit lazy val arbHeap: Arbitrary[H] = Arbitrary(genHeap)

  property("gen1") = forAll { (h: H) =>
    val m = if (isEmpty(h)) 0 else findMin(h)
    findMin(insert(m, h)) == m
  }

  property(" insert any two elements into an empty heap, finding the minimum of the resulting heap should get the smallest of the two elements back.") =  forAll {
    (num1: A , num2: A) =>
      val h = insert(num1 , insert(num2 , empty))
      findMin(h) == min(num1 , num2)
  }

  property(" insert an element into an empty heap, then delete the minimum, the resulting heap should be empty.") = forAll {
    (num: A) => isEmpty(deleteMin(insert(num , empty)))
  }

  property("sorted sequence of elements when continually finding and deleting minima.") = forAll {
    (h: H) =>
      def sorted(h: H): Boolean = {
        if(isEmpty(h)) true
        else {
          val smallestOfcurrent = findMin(h)
          val hNext = deleteMin(h)
          isEmpty(hNext) || ( smallestOfcurrent <= findMin(hNext) && sorted(hNext) )
        }
      }
      sorted(h)
  }

  property("Finding a minimum of the melding of any two heaps should return a minimum of one or the other.") = forAll {
    (h1: H , h2: H) => findMin(meld(h1 , h2)) ==  min(findMin(h1) , findMin(h2))
  }

  property("Two heaps should be equal if recursivly removing min elements result in same elements until empty") = forAll { (h1: H, h2: H) =>
    def heapEqual(h1: H, h2: H): Boolean =
      if (isEmpty(h1) && isEmpty(h2)) true
      else {
        val m1 = findMin(h1)
        val m2 = findMin(h2)
        m1 == m2 && heapEqual(deleteMin(h1), deleteMin(h2))
      }
    heapEqual(meld(h1, h2),
      meld(deleteMin(h1), insert(findMin(h1), h2)))
  }
}
