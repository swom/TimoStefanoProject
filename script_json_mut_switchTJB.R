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

setwd("C:/Users/Timo van Eldijk/Desktop/Mutswitchdata/14-12-2021")

rep=1
a=list()
filename=c ("1-1_","1-2-1_","1-5-1_", "1-5-5-1_", "1-10-2-1_")
threshold=0.9
timeslicefitplot=2000
timeslicetimetoadapt=100000
timeslicecompare=20000
maxtime=1000000

storage=data.frame()
for (nettype in 1:5){
starttimes=data.frame()
endtimes=data.frame()


for (rep in 1:11){


results <- fromJSON(file = paste(filename[nettype],rep-1,"_sigmoid", ".json", sep=""))

simple_res = rowid_to_column(as_tibble(results[c("m_avg_fitnesses","m_env_values", "m_var_fitnesses")]),var = "gen")

 

a[[paste("p", rep,1,sep="")]]=ggplot(data = simple_res %>% slice_min(gen,n = timeslicefitplot))+
  geom_rect(aes(xmin = gen - 1, xmax = gen,
                ymin = 0, ymax = 1,
                fill = as.factor(m_env_values),
                alpha = 0.5), show.legend = FALSE)+
geom_line(aes(x = gen, y = m_avg_fitnesses), show.legend = FALSE)+
geom_hline(yintercept=threshold)





a[[paste("p", rep,2,sep="")]]=ggplot(data = simple_res %>% slice_max(gen,n = timeslicefitplot), show.legend = FALSE)+
  geom_line(aes(x = gen, y = m_avg_fitnesses)) +
  geom_rect(aes(xmin = gen - 1, xmax = gen,
                ymin = 0, ymax = 1,
                fill = as.factor(m_env_values),
                alpha = 0.5), show.legend = FALSE)+
geom_hline(yintercept=threshold)

output=Timetochangecalc(threshold, 1, maxtime, simple_res) 

a[[paste("p", rep,3,sep="")]]=ggplot(data=output[output$Gen<timeslicetimetoadapt,])+geom_point(aes(x=Gen, y=Time_to_adaptation, colour=Currentenv), show.legend = FALSE )

starttimes=rbind (starttimes, output[output$Gen<timeslicecompare,])
endtimes=rbind(endtimes, output[output$Gen>(maxtime-timeslicecompare),])

}#end rep loop



AllPlots=grid.arrange(grobs=a, ncol=3)

ggsave( filename = paste(filename[nettype],".jpg"  ,sep=""),
        plot=AllPlots, width = 20, height = 60, limitsize = FALSE)


starttimes$type=paste(filename[nettype], "1start")
endtimes$type=paste (filename[nettype], "2end")

storage=rbind(storage, starttimes, endtimes)

}#end nettype loop


boxplot=ggplot (data = storage, aes(x=type, y=Time_to_adaptation))+geom_boxplot()
ggsave( "Comparing_time_to_adaptation.jpg", plot=boxplot, width =10, height=10, limitsize=F)





subdata=subset(storage, type =="1-2-1_ 2end"| type=="1-10-2-1_ 2end"| type=="1-5-5-1_ 2end"|type=="1-5-1_ 2end")

boxplot=ggplot (data = subdata, aes(x=type, y=Time_to_adaptation))+geom_boxplot()
ggsave( "Comparing_time_to_adaptation_endzoom.jpg", plot=boxplot, width =10, height=10, limitsize=F)





