library(rjson)
library(ggplot2)
library(tibble)
library(dplyr)
library(gridExtra)

Timetochangecalc = function (threshold,lbound, ubound, simple_res){
  curenv=0
  timing=FALSE
  stor=data.frame()
  for (i in lbound:ubound){
  
    if (curenv!=simple_res$m_env_values[i]) {
      curenv=simple_res$m_env_values[i]
      timing=FALSE
      if(simple_res$m_avg_fitnesses[i] < threshold){timing=TRUE}
      elapsed=0
    }
    
    if (timing == TRUE){
      elapsed=elapsed+1
      if (simple_res$m_avg_fitnesses[i] > threshold){
        timing=FALSE
        stor=rbind(stor, c(i,elapsed,simple_res$m_env_values[i]))
        
      }
    }  
    
  }
  colnames(stor)=c("Gen", "Time_to_adaptation", "Currentenv")
  return (stor)
}

############################################# end of preamble ###############

setwd("C:/Users/Timo van Eldijk/Desktop/Mutswitchdata/Datamutswitch22072021")

rep=1
a=list()

for (rep in 1:11){

results <- fromJSON(file =paste("1-2-1_",rep-1,".json", sep=""))

simple_res = rowid_to_column(as_tibble(results[c("m_avg_fitnesses","m_env_values", "m_var_fitnesses")]),var = "gen")

output=Timetochangecalc(0.9, 1, 100000, simple_res)  

a[[paste("p", rep,1,sep="")]]=ggplot(data = simple_res %>% slice_min(gen,n = 50000))+
  geom_rect(aes(xmin = gen - 1, xmax = gen,
                ymin = 0, ymax = 1.5,
                fill = as.factor(m_env_values),
                alpha = 0.5), show.legend = FALSE)+
geom_line(aes(x = gen, y = m_avg_fitnesses), show.legend = FALSE)





a[[paste("p", rep,2,sep="")]]=ggplot(data = simple_res %>% slice_max(gen,n = 1000), show.legend = FALSE)+
  geom_line(aes(x = gen, y = m_avg_fitnesses)) +
  geom_rect(aes(xmin = gen - 1, xmax = gen,
                ymin = 0, ymax = 1.5,
                fill = as.factor(m_env_values),
                alpha = 0.5), show.legend = FALSE)




a[[paste("p", rep,3,sep="")]]=ggplot(data=output)+geom_point(aes(x=Gen, y=Time_to_adaptation, colour=Currentenv), show.legend = FALSE )
}



grid.arrange(grobs=a, ncol=3)








