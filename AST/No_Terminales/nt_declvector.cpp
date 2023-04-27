#include "nt_declvector.h"

NT_DeclVector::NT_DeclVector(AbstractExpr* tipo, AbstractExpr* id, const QVector<AbstractExpr*>& expr)
    : tipo(tipo), ID(id), Expr(expr) {}

NT_DeclVector::NT_DeclVector(AbstractExpr* tipo, AbstractExpr* id)
    : tipo(tipo), ID(id), Expr({}) {}

Resultado* NT_DeclVector::Interpretar(Environment* ctx, EnvironmentFunc* ctx2, EnvironmentVect* ctx3) {
    Resultado* idR = this->ID->Interpretar(ctx, ctx2, ctx3);
    Resultado* tipoR = this->tipo->Interpretar(ctx, ctx2, ctx3);

    QString varName = idR->getValor().toString();
    std::string valueType = tipoR->getTipo().toStdString();

    QVector<Value> values;
    for (int i = 0; i < this->Expr.size(); i++) {
        Resultado* exprR = this->Expr[i]->Interpretar(ctx, ctx2, ctx3);
        if (exprR->getTipo() == tipoR->getTipo()) {
            if (valueType == "Integer") {
                values.append(exprR->getValor().toInt());
            } else if (valueType == "Float") {
                values.append(static_cast<float>(exprR->getValor().toDouble()));
            } else if (valueType == "Boolean") {
                values.append(exprR->getValor().toBool());
            } else if (valueType == "String") {
                values.append(exprR->getValor().toString().toStdString());
            }
        } else {
            // Handle type mismatch error
            // You can return an error Resultado or throw an exception
            // depending on your error handling strategy
            return nullptr;
        }
    }

    ctx3->addVariable(varName.toStdString(), valueType, values);
    return nullptr;
}

QString NT_DeclVector::Graficar() {
    std::stringstream  pPosicion, idPosicion, ePosicion, tPosicion ;
    pPosicion <<(void*)this;
    idPosicion <<(void*)this->ID;
    //ePosicion <<(void*)this->Expr;
    tPosicion <<(void*)this->tipo;

    QString nodo = QString::fromStdString("n" + pPosicion.str() +
            " [ label=\"Decl VECTOR\" , fillcolor=red];\n");
    nodo +=  QString::fromStdString("n" + pPosicion.str() + " -> n"
            + idPosicion.str()+ ";\n");
    nodo += this->ID->Graficar();
    nodo +=  QString::fromStdString("n" + pPosicion.str() + " -> n"
            + tPosicion.str()+ ";\n");
    nodo += this->tipo->Graficar();

    /*if (this->Expr != nullptr){
        nodo +=  QString::fromStdString("n" + pPosicion.str() + " -> n"
                + ePosicion.str()+ ";\n");
        nodo += this->Expr->Graficar();
    }*/
    return nodo;
}
