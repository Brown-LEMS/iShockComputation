% Specify the folder where the files live.
myFolder = '/Users/julieliu/Downloads/se_msel_count/top_160';
avg = 0;
% Get a list of all files in the folder with the desired file name pattern.
filePattern = fullfile(myFolder, '*.cemv'); % Change to whatever pattern you need.
filelist = dir(filePattern);
for k = 1 : length(filelist)
  [filepath,name,ext] = fileparts(filelist(k).name);
  fullFileName = fullfile(myFolder, filelist(k).name);
  % fprintf(1, 'Now reading %s\n', fullFileName);
  % Now do whatever you want with this file name,
  % such as reading it in as an image array with imread()
  fid = fopen(fullFileName);
  edge=0;
  while(~feof(fid))
      line = fgetl(fid);
      if length(line) > 7 & strcmp(line(1:7),' [0, 0]')== 1
          edge = edge + 1;
      end
  end
  fclose(fid);
  avg = avg + edge;         
  %imageArray = imread(fullFileName);
  %imshow(imageArray);  % Display image.
  %drawnow; % Force display to update immediately.
end

avg = avg/length(filelist);
fprintf('length = %f\n', avg);