package calculator

sealed abstract class Expr
final case class Literal(v: Double) extends Expr
final case class Ref(name: String) extends Expr
final case class Plus(a: Expr, b: Expr) extends Expr
final case class Minus(a: Expr, b: Expr) extends Expr
final case class Times(a: Expr, b: Expr) extends Expr
final case class Divide(a: Expr, b: Expr) extends Expr

object Calculator {
  def computeValues(
      namedExpressions: Map[String, Signal[Expr]]): Map[String, Signal[Double]] = {
    namedExpressions.map({
      case (stringName , expretion) => {
        stringName -> Signal(eval(expretion() , namedExpressions))
      }
    })
  }

  def eval(expr: Expr, references: Map[String, Signal[Expr]]): Double = expr match {
    case Literal(num) => num
    case Ref(myName) => eval(getReferenceExpr(myName , references) , references.filterKeys(_!=myName))
    case Plus(num1 , num2) => eval(num1,references) + eval(num2,references)
    case Minus(num1 , num2) => eval(num1,references) - eval(num2,references)
    case Times(num1 , num2) => eval(num1,references) * eval(num2,references)
    case Divide(num1 , num2) => eval(num1,references) / eval(num2,references)
    case _ => Double.NaN
  }

  /** Get the Expr for a referenced variables.
   *  If the variable is not known, returns a literal NaN.
   */
  private def getReferenceExpr(name: String,
      references: Map[String, Signal[Expr]]) = {
    references.get(name).fold[Expr] {
      Literal(Double.NaN)
    } { exprSignal =>
      exprSignal()
    }
  }
}
