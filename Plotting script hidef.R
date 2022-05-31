library(rjson)
library(ggplot2)
library(tibble)
library(dplyr)
library(gridExtra)
library(moments)
#setwd("C:/Users/Timo van Eldijk/Desktop/Mutswitchdata/10-03-2022 (with mutspec)")
setwd("C:/Users/P287139/Desktop/Mutswitch")




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


############################1-1-############################################

setwd("C:/Users/P287139/Desktop/Mutswitch")

filename="1-1_1_sigmoid.json"

results <- fromJSON(file = filename)

setwd("C:/Users/P287139/Desktop/Mutswitch/1-1hidef")

############ Plotting adaptive trajectories and time to adaptation ##############################
threshold=0.9
timeslicefitplot=180
timeslicetimetoadapt=1000000
timeslicecompare=20000
maxtime=1000000
rep=1
a=list()


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



AllPlots=grid.arrange(grobs=a, ncol=3)

ggsave( filename = paste(filename,"hidefzoom.jpg"  ,sep=""),
        plot=AllPlots, width = 20, height = 10, limitsize = FALSE)

########################################################################################



for (gennr in (1000-180):1000) {
  
  jpeg(paste (filename,"gen",gennr,"_biases.jpeg"), width = 2000, height=1000, res=200)
  par(mfrow = c(2, 2))
  ####Plot bias mutspecs####
  for (layernr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_biases))
  {
    for (biasnr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_biases[[layernr]]))
    { 
      
      
      test=results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_biases[[layernr]][[biasnr]]$m_histogram
      
      curroutput=results$m_top_inds_spectrum[[gennr]][[1]]$ind_output
      
      
      x=(sapply(test,"[[",1))-0.01
      y=sapply(test,"[[",2)
      
      print (c(layernr, biasnr))
      plot (y~x, type="l", main = paste("Bias_layer",layernr,"_nr",biasnr ))
      abline(v=curroutput, col="red")
    }
  }
  dev.off()
  
  
  jpeg(paste (filename,"gen",gennr,"_Weights.jpeg"), width = 4000, height=4000, res=200)
  par(mfrow = c(2, 2)) 
  
  
  
  ####Plot weight mutspecs####
  for (layernr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights))
  {
    
    
    for (nodenr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights[[layernr]]))
    { 
      for (weightnr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights[[layernr]][[nodenr]]))
      { 
        
        
        test=results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights[[layernr]][[nodenr]][[weightnr]]$m_histogram
        x=(sapply(test,"[[",1))-0.01
        y=sapply(test,"[[",2)
        
        curroutput=results$m_top_inds_spectrum[[gennr]][[1]]$ind_output
        
        print (c(layernr, nodenr, weightnr))
        
        plot (y~x, type="l", main = paste("w_lay",layernr,"_nod_", nodenr,"_nr",weightnr ))
        abline(v=curroutput, col="red")
        
        
      }
    } 
  }
  
  dev.off() 
  
}#end genloop




############################1-2-1-############################################

setwd("C:/Users/P287139/Desktop/Mutswitch")

filename="1-2-1_1_sigmoid.json"

results <- fromJSON(file = filename)

setwd("C:/Users/P287139/Desktop/Mutswitch/1-2-1hidef")

############ Plotting adaptive trajectories and time to adaptation ##############################
threshold=0.9
timeslicefitplot=180
timeslicetimetoadapt=1000000
timeslicecompare=20000
maxtime=1000000
rep=1
a=list()


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



AllPlots=grid.arrange(grobs=a, ncol=3)

ggsave( filename = paste(filename,"hidefzoom.jpg"  ,sep=""),
        plot=AllPlots, width = 20, height = 10, limitsize = FALSE)

########################################################################################



for (gennr in (1000-180):1000) {
  
  jpeg(paste (filename,"gen",gennr,"_biases.jpeg"), width = 1000, height=1000, res=200)
  par(mfrow = c(2, 2))
  ####Plot bias mutspecs####
  for (layernr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_biases))
  {
    for (biasnr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_biases[[layernr]]))
    { 
      
      
      test=results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_biases[[layernr]][[biasnr]]$m_histogram
      
      curroutput=results$m_top_inds_spectrum[[gennr]][[1]]$ind_output
      
      
      x=(sapply(test,"[[",1))-0.01
      y=sapply(test,"[[",2)
      
      print (c(layernr, biasnr))
      plot (y~x, type="l", main = paste("Bias_layer",layernr,"_nr",biasnr ))
      abline(v=curroutput, col="red")
    }
  }
  dev.off()
  
  
  jpeg(paste (filename,"gen",gennr,"_Weights.jpeg"), width = 1000, height=1000, res=200)
  par(mfrow = c(2, 2)) 
  
  
  
  ####Plot weight mutspecs####
  for (layernr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights))
  {
    
    
    for (nodenr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights[[layernr]]))
    { 
      for (weightnr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights[[layernr]][[nodenr]]))
      { 
        
        
        test=results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights[[layernr]][[nodenr]][[weightnr]]$m_histogram
        x=(sapply(test,"[[",1))-0.01
        y=sapply(test,"[[",2)
        
        curroutput=results$m_top_inds_spectrum[[gennr]][[1]]$ind_output
        
        print (c(layernr, nodenr, weightnr))
        
        plot (y~x, type="l", main = paste("w_lay",layernr,"_nod_", nodenr,"_nr",weightnr ))
        abline(v=curroutput, col="red")
        
        
      }
    } 
  }
  
  dev.off() 
  
}#end genloop






############################1-5-5-############################################
  
setwd("C:/Users/P287139/Desktop/Mutswitch")

filename="1-5-5-1_1_sigmoid.json"
  
results <- fromJSON(file = filename)

setwd("C:/Users/P287139/Desktop/Mutswitch/1-5-5-1hidef")

############ Plotting adaptive trajectories and time to adaptation ##############################
threshold=0.9
timeslicefitplot=180
timeslicetimetoadapt=1000000
timeslicecompare=20000
maxtime=1000000
rep=1
a=list()


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



AllPlots=grid.arrange(grobs=a, ncol=3)

ggsave( filename = paste(filename,"hidefzoom.jpg"  ,sep=""),
        plot=AllPlots, width = 20, height = 10, limitsize = FALSE)

########################################################################################
  

  
for (gennr in (1000-180):1000) {

jpeg(paste (filename,"gen",gennr,"_biases.jpeg"), width = 2000, height=1000, res=200)
par(mfrow = c(2, 6))
####Plot bias mutspecs####
for (layernr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_biases))
  {
for (biasnr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_biases[[layernr]]))
  { 


test=results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_biases[[layernr]][[biasnr]]$m_histogram

curroutput=results$m_top_inds_spectrum[[gennr]][[1]]$ind_output


x=(sapply(test,"[[",1))-0.01
y=sapply(test,"[[",2)

print (c(layernr, biasnr))
plot (y~x, type="l", main = paste("Bias_layer",layernr,"_nr",biasnr ))
abline(v=curroutput, col="red")
}
}
dev.off()


jpeg(paste (filename,"gen",gennr,"_Weights.jpeg"), width = 4000, height=4000, res=200)
par(mfrow = c(6, 6)) 
 


####Plot weight mutspecs####
  for (layernr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights))
  {
  
    
    for (nodenr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights[[layernr]]))
    { 
      for (weightnr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights[[layernr]][[nodenr]]))
      { 
   
      
      test=results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights[[layernr]][[nodenr]][[weightnr]]$m_histogram
      x=(sapply(test,"[[",1))-0.01
      y=sapply(test,"[[",2)
      
      curroutput=results$m_top_inds_spectrum[[gennr]][[1]]$ind_output
      
      print (c(layernr, nodenr, weightnr))
      
      plot (y~x, type="l", main = paste("w_lay",layernr,"_nod_", nodenr,"_nr",weightnr ))
      abline(v=curroutput, col="red")
      
      
    }
    } 
  }

dev.off() 

}#end genloop













############################## 1-10-2-1##############


setwd("C:/Users/P287139/Desktop/Mutswitch")

filename="1-10-2-1_1_sigmoid.json"


results <- fromJSON(file = filename)

setwd("C:/Users/P287139/Desktop/Mutswitch/1-10-2-1hidef")

############ Plotting adaptive trajectories and time to adaptation ##############################
threshold=0.9
timeslicefitplot=180
timeslicetimetoadapt=1000000
timeslicecompare=20000
maxtime=1000000
rep=1
a=list()


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



AllPlots=grid.arrange(grobs=a, ncol=3)

ggsave( filename = paste(filename,"hidefzoom.jpg"  ,sep=""),
        plot=AllPlots, width = 20, height = 10, limitsize = FALSE)

########################################################################################



for (gennr in (1000-180):1000) {

jpeg(paste (filename,"gen",gennr,"_biases.jpeg"), width = 3000, height=1000, res=200)
par(mfrow = c(2, 7))
####Plot bias mutspecs####
for (layernr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_biases))
{
  for (biasnr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_biases[[layernr]]))
  { 
    
    
    test=results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_biases[[layernr]][[biasnr]]$m_histogram
    
    curroutput=results$m_top_inds_spectrum[[gennr]][[1]]$ind_output
    
    
    x=(sapply(test,"[[",1))-0.01
    y=sapply(test,"[[",2)
    
    print (c(layernr, biasnr))
    plot (y~x, type="l", main = paste("Bias_layer",layernr,"_nr",biasnr ))
    abline(v=curroutput, col="red")
  }
}
dev.off()


jpeg(paste (filename,"gen",gennr,"_Weights.jpeg"), width = 4000, height=4000, res=200)
par(mfrow = c(6, 6)) 



####Plot weight mutspecs####
for (layernr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights))
{
  
  
  for (nodenr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights[[layernr]]))
  { 
    for (weightnr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights[[layernr]][[nodenr]]))
    { 
      
      
      test=results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights[[layernr]][[nodenr]][[weightnr]]$m_histogram
      x=(sapply(test,"[[",1))-0.01
      y=sapply(test,"[[",2)
      
      curroutput=results$m_top_inds_spectrum[[gennr]][[1]]$ind_output
      
      print (c(layernr, nodenr, weightnr))
      
      plot (y~x, type="l", main = paste("w_lay",layernr,"_nod_", nodenr,"_nr",weightnr ))
      abline(v=curroutput, col="red")
      
      
    }
  } 
}

dev.off() 

}#endgenloop


















############################1-1 target-############################################

setwd("C:/Users/P287139/Desktop/Mutswitch")

filename="1-1_1_sigmoidtarget1.json"

results <- fromJSON(file = filename)

setwd("C:/Users/P287139/Desktop/Mutswitch/1-1hidef")

############ Plotting adaptive trajectories and time to adaptation ##############################
threshold=0.9
timeslicefitplot=180
timeslicetimetoadapt=1000000
timeslicecompare=20000
maxtime=1000000
rep=1
a=list()


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



AllPlots=grid.arrange(grobs=a, ncol=3)

ggsave( filename = paste(filename,"hidefzoom.jpg"  ,sep=""),
        plot=AllPlots, width = 20, height = 10, limitsize = FALSE)

########################################################################################



for (gennr in (1000-180):1000) {
  
  jpeg(paste (filename,"gen",gennr,"_biases.jpeg"), width = 2000, height=1000, res=200)
  par(mfrow = c(2, 2))
  ####Plot bias mutspecs####
  for (layernr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_biases))
  {
    for (biasnr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_biases[[layernr]]))
    { 
      
      
      test=results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_biases[[layernr]][[biasnr]]$m_histogram
      
      curroutput=results$m_top_inds_spectrum[[gennr]][[1]]$ind_output
      
      
      x=(sapply(test,"[[",1))-0.01
      y=sapply(test,"[[",2)
      
      print (c(layernr, biasnr))
      plot (y~x, type="l", main = paste("Bias_layer",layernr,"_nr",biasnr ))
      abline(v=curroutput, col="red")
    }
  }
  dev.off()
  
  
  jpeg(paste (filename,"gen",gennr,"_Weights.jpeg"), width = 4000, height=4000, res=200)
  par(mfrow = c(2, 2)) 
  
  
  
  ####Plot weight mutspecs####
  for (layernr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights))
  {
    
    
    for (nodenr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights[[layernr]]))
    { 
      for (weightnr in 1:length (results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights[[layernr]][[nodenr]]))
      { 
        
        
        test=results$m_top_inds_spectrum[[gennr]][[1]]$mutational_spectrum$m_outputs_of_mutated_weights[[layernr]][[nodenr]][[weightnr]]$m_histogram
        x=(sapply(test,"[[",1))-0.01
        y=sapply(test,"[[",2)
        
        curroutput=results$m_top_inds_spectrum[[gennr]][[1]]$ind_output
        
        print (c(layernr, nodenr, weightnr))
        
        plot (y~x, type="l", main = paste("w_lay",layernr,"_nod_", nodenr,"_nr",weightnr ))
        abline(v=curroutput, col="red")
        
        
      }
    } 
  }
  
  dev.off() 
  
}#end genloop
























