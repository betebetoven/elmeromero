#include "nt_Bloque.h"
#include "nt_declvar.h"
#include"nt_declvar.h"

Bloque::Bloque(const QVector<AbstractExpr*>& instrucciones, AbstractExpr *decl, AbstractExpr *expr, AbstractExpr *aumento, bool is_while, AbstractExpr *elsebloque) : instrucciones(instrucciones), decl(decl), expr(expr), aumento(aumento), is_while(is_while), elsebloque(elsebloque)
{this->declaracionparametros={};this->asignacionparametros={};}

Resultado* Bloque::Interpretar(Environment* env,EnvironmentFunc* ctx2, EnvironmentVect* ctx3) {
    Environment *envv = new Environment(env);

    Resultado* temp = new Resultado(nullptr);

//ESTO ES EL BLOQUE PARA LAS FUNCIONES
    if (decl == nullptr && expr == nullptr && aumento == nullptr) {
       if (this->declaracionparametros.size() != 0)
       {
           for (int i = 0; i < this->declaracionparametros.size(); i++) {
               NT_DeclVar *t =dynamic_cast<NT_DeclVar*>( this->declaracionparametros[i]);
               //t->Expr = this->asignacionparametros[i];//lleva E al lote Expr en la declaracion para que no sea null
               temp = t->Interpretar(envv,ctx2,ctx3);
           }
       }//ESTA ES LA FUNCION SIN PARAMETROS
        for (int i = 0; i < this->instrucciones.size(); i++) {
            temp = this->instrucciones[i]->Interpretar(envv,ctx2,ctx3);

        }
    }




//ESTO ES UN WHILE O UN FOR TODAVIA NO SE JAJAJ
    else if (is_while){
        if(decl != nullptr)
        Resultado *declResult = decl->Interpretar(envv,ctx2,ctx3);
        while (true) {
            Resultado *exprResult = expr->Interpretar(envv,ctx2,ctx3);
            if (exprResult->getValor().toBool() == false) {
                    break;
            }
            for (int i = 0; i < this->instrucciones.size(); i++) {
                temp = this->instrucciones[i]->Interpretar(envv,ctx2,ctx3);
                if(temp!=nullptr)
                {
                if(temp->getValor().toString().toStdString()=="break")// aca puede ir el return como || pero primero hay que hacer las funcinoes
                {env->updateCommonVariables(envv);
                    //env->report();
                    return nullptr;}
                if(temp->getValor().toString().toStdString()=="continue"&&aumento != nullptr)
                {
                    break;

                }}
            }
            if (aumento != nullptr)
            Resultado *aumentoResult = aumento->Interpretar(envv,ctx2,ctx3);
        }
    }


 //ESTO ES UN IF_____________________________________
    else
    {
        QString temporal = "";
        QVector<QString> ev = {};
        QVector<QString> ef = {};
        QString lsalida = "";
        Resultado *exprResult = expr->Interpretar(envv,ctx2,ctx3);

        if(exprResult->miniResultado.temporales.size()!=0)
        {
             temporal = exprResult->miniResultado.temporales[0];
             ev.push_front( "L"+QString::number(MiniResultado::L++));
            ef.push_front( "L"+QString::number(MiniResultado::L++));
            std::cout<<"if("<<temporal.toStdString()<<") then goto "<<ev[0].toStdString()<<";"<<std::endl;
            std::cout<<"goto "<<ef[0].toStdString()<<std::endl;
            std::cout<<ev[0].toStdString()<<":"<<std::endl;
        }
        else if(exprResult->miniResultado.EV.size() != 0 && exprResult->miniResultado.EF.size()!=0)
        {
            ev=exprResult->miniResultado.EV;
           ef=exprResult->miniResultado.EF;
           std::cout<<ev[0].toStdString()<<":"<<std::endl;

        }
        else
        {
            temporal = QString::number(exprResult->getValor().toBool());
            ev.push_front( "L"+QString::number(MiniResultado::L++));
           ef.push_front( "L"+QString::number(MiniResultado::L++));
           std::cout<<"if("<<temporal.toStdString()<<") then goto "<<ev[0].toStdString()<<";"<<std::endl;
           std::cout<<"goto "<<ef[0].toStdString()<<std::endl;
           std::cout<<ev[0].toStdString()<<":"<<std::endl;
        }
        //esto es el c3d sentencias


            for (int i = 0; i < this->instrucciones.size(); i++)
            {
                temp = this->instrucciones[i]->Interpretar(envv,ctx2,ctx3);
                if(temp!=nullptr)
                {
                if(temp->getValor().toString().toStdString()=="break")
                    {env->updateCommonVariables(envv);
                    //env->report();
                    return new Resultado(QString::fromStdString("break"));
                    }
                if(temp->getValor().toString().toStdString()=="continue"&&aumento != nullptr)
                    {
                    env->updateCommonVariables(envv);
                    //env->report();
                    return new Resultado(QString::fromStdString("continue"));

                    }
                }
            }
            lsalida = "L"+QString::number(MiniResultado::L++);
            std::cout<<"goto "<<lsalida.toStdString()<<";"<<std::endl;

        //else

            for (int var = 0; var < ef.size(); ++var) {
            std::cout<<ef[var].toStdString()<<":"<<std::endl;
            }

        //c3d de else
            if(elsebloque != nullptr)
        {
            Resultado *aux = elsebloque->Interpretar(env,ctx2,ctx3);
            //return aux;

        }

           std::cout<<lsalida.toStdString()<<":"<<std::endl;



    }

    env->updateCommonVariables(envv);
    //env->report();
    //envv->report();


    return temp;
}

QString Bloque::Graficar() {
    std::stringstream  pPosicion, lPosicion,elseposicion;
    pPosicion <<(void*)this;
    QString temp = QString::fromStdString("n" + pPosicion.str()  +
                " [ label=\"BLOQUE\" fillcolor=springgreen];\n");


    for (int i = 0; i < this->instrucciones.size(); i ++){
        lPosicion.str(std::string());
        lPosicion.clear();
        lPosicion << (void*)this->instrucciones[i];

        temp +=  QString::fromStdString("n" + pPosicion.str() + " -> n"
                        + lPosicion.str()+ ";\n");
      temp += this->instrucciones[i]->Graficar();
      if(this->elsebloque != nullptr)
      {
          lPosicion.str(std::string());
          lPosicion.clear();
          lPosicion << (void*)this->elsebloque;

          temp +=  QString::fromStdString("n" + pPosicion.str() + " -> n"
                          + lPosicion.str()+ ";\n");
          temp+=this->elsebloque->Graficar();
      }
        /// es un return? es un break, es un continue, etc....
    }
    std::cout << "alv si esta vacio" << std::endl;
    return temp;
}
