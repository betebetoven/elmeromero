#include "nt_llamada.h"
#include "nt_bloque.h"

Resultado* NT_Llamada::Interpretar(Environment* env, EnvironmentFunc* ctx2, EnvironmentVect* ctx3) {
    Resultado* idR = this->ID->Interpretar(env,ctx2,ctx3);
    std::string varName = idR->getValor().toString().toStdString();
    this->nombre = varName;
    AbstractExpr* expr = ctx2->getVariableExpr(varName);
    
    if (expr) {
        // Call the Interpretar method of the retrieved AbstractExpr object
        if(this->asignacionparametros.size()!= 0)
        {
        Bloque *t =dynamic_cast<Bloque*>(expr);
        t->asignacionparametros = this->asignacionparametros;
        return t->Interpretar(env, ctx2,ctx3);
        }





        return expr->Interpretar(env, ctx2,ctx3);
    } 
    else {
        // Throw an error
        return nullptr;
    }
}

QString NT_Llamada::Graficar() {
    std::stringstream pPosicion;
    pPosicion << (void*)this;
    return QString::fromStdString("n" + pPosicion.str() +
                                  " [ label=\"EXP: Llamada " + this->nombre +
                                  "\"   fillcolor=lightblue];\n");
}

NT_Llamada::NT_Llamada(AbstractExpr *id) : ID(id) {this-> asignacionparametros = {};}
NT_Llamada::NT_Llamada(AbstractExpr *id,const QVector<AbstractExpr*>& asignacionparametros) : ID(id), asignacionparametros(asignacionparametros) {}
