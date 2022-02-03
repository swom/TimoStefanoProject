library(rjson)
library(ggplot2)
library(tibble)
library(dplyr)
library(gridExtra)
library(moments)
setwd("C:/Users/Timo van Eldijk/Desktop/Mutswitchdata/2-02-2022 (with biases)")

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



netcalc=function (W1, W2, W3, W4, B1, B2, B3){
  Input=1
  Inputnode1=(Input*W1)+B1
  Inputnode2=(Input*W2)+B2
  Outputnode1= (Inputnode1)/(1+ abs((Inputnode1)))
  Outputnode2=(Inputnode2)/(1+ abs((Inputnode2)))
  Inputfinalnode=(Outputnode1*W3)+(Outputnode2*W4)+B3
  Finaloutput=(Inputfinalnode)/(1+abs(Inputfinalnode))
  return(Finaloutput)
}


############################################# end of preamble ###############



rep=1

filename=c ("1-1_","1-2-1_","1-5-1_", "1-5-5-1_", "1-10-2-1_")
threshold=0.9
timeslicefitplot=2000
timeslicetimetoadapt=1000000
timeslicecompare=20000
maxtime=1000000

storage=data.frame()
for (nettype in 1:5){
starttimes=data.frame()
endtimes=data.frame()
a=list()

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






#####Plotting the weights of 10 best individuals#######

filename=c ("1-1_","1-2-1_","1-5-1_", "1-5-5-1_", "1-10-2-1_")
nettype=2
rep=2

for (rep in 1:11){

results <- fromJSON(file = paste(filename[nettype],rep-1,"_sigmoid", ".json", sep=""))
test=results$m_top_inds

a=list()
for (indiv in 1:10) {
  
stor=data.frame()
for (time in 1:1000){
  stor=rbind (stor,c(time, unlist(test[[time]][[indiv]]$m_network$m_network_weights), unlist(test[[time]][[indiv]]$m_network$m_nodes_biases)))
}

colnames(stor) <- c("time", "w1", "w2","w3","w4", "B1", "B2", "B3")

a[[paste("p", rep,indiv,sep="")]]=ggplot(data = stor )+
  labs (y="Weight", x="time")+
  geom_point(aes (x=time, y=w1, color="w1"))+
  geom_point(aes (x=time, y=w2, color="w2"))+
  geom_point(aes (x=time, y=w3, color="w3"))+
  geom_point(aes (x=time, y=w4, color="w4"))+
  geom_point(aes (x=time, y=B1, color="B1"))+
  geom_point(aes (x=time, y=B2, color="B2"))+
  geom_point(aes (x=time, y=B3, color="B3"))

}#endindivloop
AllPlots=grid.arrange(grobs=a, ncol=2)
ggsave( filename = paste("weighplot ",filename[nettype],"rep", rep ,".jpg"  ,sep=""),
        plot=AllPlots, width = 10, height = 15, limitsize = FALSE)
}#end reploop



#####Plotting the weights of only the best individuals#######

filename=c ("1-1_","1-2-1_","1-5-1_", "1-5-5-1_", "1-10-2-1_")
nettype=2
a=list()
indiv=1
for (rep in 1:11){
  results <- fromJSON(file = paste(filename[nettype],rep-1,"_sigmoid", ".json", sep=""))
  test=results$m_top_inds
    stor=data.frame()
    for (time in 1:1000){
      stor=rbind (stor,c(time, unlist(test[[time]][[indiv]]$m_network$m_network_weights), unlist(test[[time]][[indiv]]$m_network$m_nodes_biases)))
    }
    
    colnames(stor) <- c("time", "w1", "w2","w3","w4", "B1", "B2", "B3")
    
    a[[paste("p", rep,sep="")]]=ggplot(data = stor )+
      labs (y="Weight", x="time")+
      geom_point(aes (x=time, y=w1, color="w1"))+
      geom_point(aes (x=time, y=w2, color="w2"))+
      geom_point(aes (x=time, y=w3, color="w3"))+
      geom_point(aes (x=time, y=w4, color="w4"))+
      geom_point(aes (x=time, y=B1, color="B1"))+
      geom_point(aes (x=time, y=B2, color="B2"))+
      geom_point(aes (x=time, y=B3, color="B3"))
    
}#end reploop
AllPlots=grid.arrange(grobs=a, ncol=2)
  ggsave( filename = paste("weighplot best only all reps first 100",filename[nettype] ,".jpg"  ,sep=""),
          plot=AllPlots, width = 10, height = 15, limitsize = FALSE)


  
  
  
  
  #######Weight and mutspec examination#######

  
  for (rep in 1:11){
  
  filename=c ("1-1_","1-2-1_","1-5-1_", "1-5-5-1_", "1-10-2-1_")
  nettype=2
  a=list()
  indiv=1
  results <- fromJSON(file = paste(filename[nettype],rep-1,"_sigmoid", ".json", sep=""))
  test=results$m_top_inds
  
    stor=data.frame()
  for (time in 1:1000){
    stor=rbind (stor,c(time, unlist(test[[time]][[indiv]]$m_network$m_network_weights), unlist(test[[time]][[indiv]]$m_network$m_nodes_biases)))
    
  }
    colnames(stor) <- c("time", "w1", "w2","w3","w4", "B1", "B2", "B3")
 
 
  jpeg(file=paste("1-2-1_",rep-1 , " mutspec_552.jpg"),  width = 1000, height = 800, res=100 ) 
  par(mfrow = c(2, 4)) 
  
  Input=1
  time=1:1000
  W1=stor$w1[time]
  W2=stor$w2[time]
  W3=stor$w3[time]
  W4=stor$w4[time]
  B1=stor$B1[time]
  B2=stor$B2[time]
  B3=stor$B3[time]

  Finaloutput=netcalc(W1, W2, W3, W4, B1, B2, B3)
  
  plot (Finaloutput~time, main="output through time")

  
 
  time=552
 
  W1=stor$w1[time]
  W2=stor$w2[time]
  W3=stor$w3[time]
  W4=stor$w4[time]
  B1=stor$B1[time]
  B2=stor$B2[time]
  B3=stor$B3[time]
 weightnames=c('W1', 'W2', 'W3', 'W4', 'B1', 'B2', 'B3')
 
 
  for(i in 1:7){
  old=get(weightnames[i])
  assign(weightnames[i], old+rnorm( 100000 ,0, 0.1))
  result=netcalc(W1, W2, W3, W4, B1, B2, B3)
  hist(result,breaks=100,  xlim=c(-1,1), main=paste("mutspec",weightnames[i]))
  print(weightnames[i])
  assign(weightnames[i], old)
  abline(v=netcalc(W1, W2, W3, W4, B1, B2, B3), col="Red")
  }

 dev.off() 
 
 }#end reploop
  
  
 
  
  