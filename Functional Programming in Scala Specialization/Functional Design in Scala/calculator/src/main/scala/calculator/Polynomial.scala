package calculator
import Math._
object Polynomial {
  def computeDelta(a: Signal[Double], b: Signal[Double],
      c: Signal[Double]): Signal[Double] = {
    Signal{
      pow(b() , 2) - 4 * a() * c()
    }
  }

  def computeSolutions(a: Signal[Double], b: Signal[Double],
      c: Signal[Double], delta: Signal[Double]): Signal[Set[Double]] = {
    Signal{
      val del = delta()
      if(del < 0) Set()
      else {
        val bvalue = b()
        val avalue = a()
        Set(
          (-bvalue + sqrt(del)) / (2*avalue),
          (-bvalue - sqrt(del)) / (2*avalue)
        )
      }
    }
  }
}
